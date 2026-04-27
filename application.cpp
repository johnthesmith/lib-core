#include "application.h"

#include <cstring>
#include <fstream>          /* lock */
#include <unistd.h>         /* usleep */
#include <iostream>
#include <limits.h>

#include <signal.h>
#include <termios.h>

#include "utils.h"
#include "payload.h"




/*
    Статический указатель на текущее приложение для обработки сигналов
    Без этого будет ошибка линковки
*/
Application* Application::application = NULL;

/*
    Constructor
*/
Application::Application
(
    int     aCount, /* cli argumends count */
    char**  aList   /* cli arguments */
)
{
    /* Записываем указатель */
    application = this;

    /* Create base coponents */
    log         = Log::create();
    logManager  = LogManager::create( log );
    config      = ParamListFile::create();
    cli         = ParamList::create();

    /* Fill config from cli */
    for( int i = 1; i < aCount; i++ )
    {
        string arg = aList[ i ];
        auto lexemes = explode( arg, (string) "=", true, 1 );
        string key = "";
        string value = "";

        /* Split on key and value */
        if( lexemes.size() == 1 )
        {
            key = lexemes[ 0 ];
        }
        else
        {
            key = lexemes[ 0 ];
            value = lexemes[ 1 ];
        }

        /* Remove -- from key*/
        if( key.find( "--" ) == 0 )
        {
            key = explode( key, "--", true, 1)[ 1 ];
        }

        /* Write to config */
        cli -> setValue( key, value );
    }

    prepareConfiguration();

    /* Create mon */
    mon         = Mon::create( cli -> getString( Path{ "mon" }, "default.mon" ));

    /* Set log destination */
    getLog() -> setFileName( cli -> getString( Path{ "log" }, "" ) );
}



/*
    Destructor
*/
Application::~Application()
{
    /* Destroy all objects */

    mon         -> destroy();
    cli         -> destroy();
    config      -> destroy();
    log         -> destroy();
    logManager  -> destroy();

    /*
        Восстанавливаем стандартные обработчики для всех зарегистрированных
        сигналов
    */
    for( int sig : registered_signals )
    {
        signal( sig, SIG_DFL );
    }
}



/*
    Creator
*/
Application* Application::create
(
    int     aCount, /* cli argumends count */
    char**  aList   /* cli arguments */
)
{
    return new Application( aCount, aList );
}



/*
    Destroy
*/
void Application::destroy()
{
    delete this;
}



/*
    Return cli config object
*/
ParamList* Application::getCli()
{
    return cli;
}



/*
    Return config object
*/
ParamListFile* Application::getConfig()
{
    return config;
}



/**********************************************************************
    Log manager operations
*/


/*
    Return Log manager
*/
LogManager* Application::getLogManager()
{
    return logManager;
}



/*
    Create new log
*/
Log* Application::createThreadLog
(
    string aId
)
{
    return getLogManager() -> createLog( aId );
}



/*
    Destroy log by id
*/
Application*  Application::destroyThreadLog()
{
    getLogManager() -> destroyLog();
    return this;
}



/*
    Return log object by id log
    or default log application
*/
Log* Application::getLog()
{
    return getLogManager() -> getLog();
}



/*
    Return mon object
*/
Mon* Application::getMon()
{
    return mon;
}




/*
    End of thread
*/
Application* Application::onThreadAfter()
{
    return this;
}



/*
    Lock process and waiting resolve locker file from user
*/
string Application::lock
(
    string          file,
    ParamList*      params,
    LockTerminated  terminated
)
{
    string result;

    if( checkPath( getPath( file )))
    {
        /* Open lock stream */
        ofstream f;
        f.open( file );
        if( f.is_open() )
        {
            f << params -> toString() << endl;
            f.close();

            /* Ожидание удаления файла */
            while( fileExists( file ) && ( terminated == NULL || !terminated() ) )
            {
                usleep( 100000 );
            }
            result = RESULT_OK;
        }
        else
        {
            result = "file_create_error";
        }
    }
    else
    {
        result = "path_create_error";
    }

    return result;
}



/*
    Check update moment of the config file.
    If file was updated, then the config object is rebuilding.
*/
Application* Application::checkConfigUpdate()
{
    string configFileName = getConfigFileName();
    if( fileExists( configFileName ))
    {
        config -> lock();

        bool cfgUpdated = checkFileUpdate( configFileName, lastConfigUpdate );

        if( cfgUpdated )
        {
            getLog()
            -> trace( "Load config file" )
            -> prm( "name", configFileName );

            /* Load config and cli */
            getConfig()
            -> clear()
            -> fromJsonFile( configFileName )
            ;

            if( getConfig() -> isOk())
            {
                getConfig() -> resultTo( this );
                getConfig() -> copyFrom( getCli() ) ;
            }
            else
            {
                getLog()
                -> warning( "Config error" )
                -> prm( "code", getConfig() -> getCode() )
                ;
            }
        }

        configUpdated = configUpdated || cfgUpdated;

        if( configUpdated )
        {
            onConfigUpdated();
        }

        config -> unlock();
    }
    else
    {
        getConfig() -> setResult( "config_not_exists" );
    }

    return this;
}



/*
    Return true if config was updated
*/
bool Application::getConfigUpdated()
{
    bool result = configUpdated;
    configUpdated = false;
    return result;
}



/*
    Return the name of configuraion file
*/
string Application::getConfigFileName()
{
    return getCli() -> getString( Path{ "config" }, "./config.json" );
}



/*
    Глобальный обработчик сигналов
*/
static void globalSignalHandler
(
    int aSignal
)
{
    if( Application::application != NULL )
    {
        Application::application -> onSignal( aSignal );
    }
}



/*
    Регистрация сигнала
    После регистрации будет вызываться globalSignalHandler onSignal
*/
void Application::registerSignal
(
    int aSignal
)
{
    registered_signals.push_back( aSignal );
    signal( aSignal, globalSignalHandler );
}




/*
    Prepare configuration for application running
*/
Application* Application::prepareConfiguration()
{
    /* Output cli arguments */
    getLog() -> begin( "CLI parameters reading" );

    for( int i = 0; i < getCli() -> getCount(); i++ )
    {
        auto param = getCli() -> getByIndex( i );
        getLog()
        -> trace( "" )
        -> prm( param -> getName(), param -> getString() );
    }
    getLog()
    -> trace( "Config source" )
    -> prm( "file", getConfigFileName() )
    -> end();

    return this;
}



/*
    Run application
*/
Application* Application::run()
{
    while( !terminated )
    {
        config -> lock();
        checkConfigUpdate();
        auto updated = getConfigUpdated();
        config -> unlock();

        /* Try to load payload if not loaded */
        if( updated )
        {
            /* Get current path */
            char cwd[ PATH_MAX ];
            std::string currentPath = "";
            if( getcwd( cwd, sizeof(cwd)) != NULL )
            {
                currentPath = std::string(cwd);
            }

            /* List of payloads from config */
            auto payloadsConf = getConfig() -> getObject
            (
                Path{ "engine", "payloads" }
            );

            if( payloadsConf != nullptr )
            {
                /*
                    Stop and unload payloads if payloads:
                        loaded but not exists in config
                        or change library
                */
                for( auto& [ key, val ] : payloads )
                {
                    auto confItem = payloadsConf -> getObject( Path{ key });
                    if
                    (
                        /* In conf not found */
                        confItem == nullptr
                        ||
                        /* Lib change */
                        (
                            confItem != nullptr &&
                            val.libraryPath != confItem -> getString( Path{ "lib" } )
                        )
                        ||
                        /* Not enabled */
                        confItem -> getBool( Path{ "enabled" }, true ) == false
                    )
                    {
                        /* Paylaod stop and unload */
                        val.instance -> destroy();
                        val.instance = nullptr;
                    }
                }

                /*
                    Payloads load if not exsist
                */
                getLog() -> begin( "Payload config" );
                payloadsConf -> loop
                (
                    [ this ]
                    ( Param* confItem )
                    {
                        if( confItem -> isObject() )
                        {
                            auto key = confItem -> getName();

                            auto lib = confItem
                            -> getObject()
                            -> getString( Path{ "lib" } );

                            auto log = confItem
                            -> getObject()
                            -> getString( Path{ "log" } );

                            auto thread = confItem
                            -> getObject()
                            -> getBool( Path{ "thread" } );

                            getLog()
                            -> begin( "item" )
                            -> prm( "name", key )
                            -> prm( "library", lib );

                            if
                            (
                                confItem
                                -> getObject()
                                -> getBool( Path{ "enabled" }, true )
                            )
                            {
                                auto item = payloads.find( key );
                                if
                                (
                                    item == payloads.end() ||
                                    (item -> second).instance == nullptr
                                )
                                {
                                    auto payload = Payload::load( lib, this, key );
                                    if( payload != nullptr )
                                    {
                                        getLog()
                                        -> trace( "Payload loaded" )
                                        -> lineEnd();

                                        payloads[ key ] = PayloadItem
                                        {
                                            lib,
                                            nullptr,
                                            payload
                                        };

                                        /* Start payload */
                                        payload -> start( thread );
                                    }
                                    else
                                    {
                                        getLog()
                                        -> warning( "Payload library not found" )
                                        -> prm( "name", lib )
                                        -> lineEnd();
                                    }
                                }
                                else
                                {
                                    getLog()
                                    -> trace( "Payload is staying" )
                                    -> lineEnd();
                                }
                            }
                            else
                            {
                                getLog()
                                -> info( "Payload disabled" )
                                -> lineEnd();
                            }
                            getLog() -> end() -> lineEnd();
                        }
                        else
                        {
                            getLog()
                            -> warning( "Not array of item" )
                            -> lineEnd();
                        }
                        return false;
                    }
                );

                /*
                    Payloads instance clear
                */
                std::map<string, PayloadItem> newPayloads;
                for( auto& [ key, val ] : payloads )
                {
                    if( val.instance != nullptr )
                    {
                        newPayloads[ key ] = val;
                    }
                }
                payloads = newPayloads;

                getLog() -> end();
            }
            else
            {
                getLog()
                -> warning( "engine.payloads argument not found" )
                -> lineEnd();
            }
        }

        usleep( 1000 );
    }

    /* All payloads stop and destroy*/
    for( auto& [ key, val ] : payloads )
    {
        if( val.instance != nullptr )
        {
            val.instance -> destroy();
        }
    }

    return this;
}
