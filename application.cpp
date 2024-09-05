#include "application.h"

#include <cstring>
#include <fstream>          /* lock */
#include <unistd.h>         /* usleep */
#include <iostream>

#include "utils.h"



/*
    Constructor
*/
Application::Application
(
    int     aCount, /* cli argumends count */
    char**  aList   /* cli arguments */
)
{
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

    /* Create mon */
    mon         = Mon::create( cli -> getString( "mon", "default.mon" ));

    /* Set log destination */
    getLog() -> setFileName( cli -> getString( "log", "" ) );
}



/*
    Destructor
*/
Application::~Application()
{
    mon         -> destroy();
    cli         -> destroy();
    config      -> destroy();
    log         -> destroy();
    logManager  -> destroy();
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
        bool cfgUpdated = checkFileUpdate( configFileName, lastConfigUpdate );

        if( cfgUpdated )
        {
            getLog()
            -> trace( "Load config file" )
            -> prm( "name", configFileName );

            /* Load config and cli */
            getConfig()
            -> clear()
            -> fromJsonFile( configFileName );

            if( getConfig() -> isOk())
            {
                getConfig() -> resultTo( this );
                getConfig() -> copyFrom( getCli() ) ;
            }
        }

        configUpdated = configUpdated || cfgUpdated;
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
    return getCli() -> getString( "config", "./config.json" );
}

