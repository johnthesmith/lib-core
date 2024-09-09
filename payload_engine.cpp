/* Local libraries */
#include "payload_engine.h"



using namespace std;



/*
    Constructor of paylod
*/
PayloadEngine::PayloadEngine
(
    Application* application
)
    : Payload( application ) // Вызов конструктора базового класса
{
}



/*
    Destructor of payload
*/
PayloadEngine::~PayloadEngine()
{
}



/*
    Creator
*/
PayloadEngine* PayloadEngine::create
(
    Application* aApplication
)
{
    return new PayloadEngine( aApplication );
}



/*
    Payload loop before default even
*/
void PayloadEngine::onLoopBefore()
{
    getMon() -> now( Path{ "startMks" }, true );
    getMon() -> now( Path{ "startMoment" }, false );
    onEngineLoopBefore();
}



/*
    Payload loop before default even
*/
void PayloadEngine::onEngineLoopBefore()
{
    /* Can be overrided in childrens */
}



/*
    Payload loop before default even
*/
void PayloadEngine::onLoop()
{
    getLog() -> trapOn() -> begin( "Loop" );

    getMon()
    -> interval( Path{ "uptime" }, Path{ "currentMks" }, Path{ "startMks" })
    -> startTimer( Path{ "loop", "moment" })
    ;

    /* Check local application config */
    getApplication()
    -> checkConfigUpdate()
    -> getConfigUpdated();

    getApplication() -> getConfig() -> resultTo( this );

    if( isOk() )
    {
        /* Check enabled */
        auto enabled = getApplication()
        -> getConfig()
        -> getBool( Path{ "engine","enabled" }, true );

        getMon() -> setBool( Path{ "enabled" }, enabled );
        if( enabled )
        {
            onEngineLoop();
        }
        else
        {
            setCode( "disabled" );
        }
    }

    /*
        Define result state action
    */
    auto code = getApplication()
    -> getConfig()
    -> getObject( Path{ "engine", "teacher", "code", getCode() });

    if( code == NULL )
    {
        /* Read default result state action */
        code = getApplication()
        -> getConfig()
        -> getObject( Path{ "engine", "teacher", "code", "*" });
    }

    if( code != NULL )
    {
        /* Log out */
        getLog()
        -> record
        (
            Log::logRecordFromString( code -> getString( "log", "ERROR" )),
            getCode()
        )
        -> text( getMessage() );

        /* Exit form payload */
        if( code -> getBool( "exit", false ))
        {
            terminate();
        }

        /* Sleep timeout */
        auto sleep = code -> getInt( "timeoutMcs", 0 );
        if( sleep != 0)
        {
            setLoopTimeoutMcs( sleep );
        }
    }
    else
    {
        getLog() -> error( "unknown_action" ) -> prm( "code", getCode() );
        terminate();
    }

    /* Final monitoring */
    getMon() -> setString( Path{ "Result" }, getCode() )-> flush();

    getLog() -> end() -> trapOff();
}




/*
    Payload engine loop default event
*/
void PayloadEngine::onEngineLoop()
{
    /* Can be overrided in childrens */
}

