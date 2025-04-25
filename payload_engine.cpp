/* Local libraries */
#include "payload_engine.h"
#include "moment.h"



using namespace std;



/*
    Constructor of paylod
*/
PayloadEngine::PayloadEngine
(
    Application* application,
    string aId
)
    : Payload( application, aId ) // Вызов конструктора базового класса
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
    Application* aApplication,
    string aId
)
{
    return new PayloadEngine( aApplication, aId );
}



/*
    Payload loop before default even
*/
void PayloadEngine::onStartBefore()
{
    getMon() -> now( Path{ "startMks" }, true );
    getMon() -> now( Path{ "startMoment" }, false );
    onEngineStartBefore();
}



/*
    Payload loop before default even
*/
void PayloadEngine::onEngineStartBefore()
{
    /* Can be overrided in childrens */
}



/*
    Payload loop before default even
*/
void PayloadEngine::onLoop()
{
    /* Check local application config */
    bool configUpdated = getApplication()
    -> checkConfigUpdate()
    -> getConfigUpdated();

    getApplication() -> getConfig() -> resultTo( this );

    if( configUpdated )
    {
        getLog() -> setTrapEnabled
        (
            getApplication()
            -> getConfig()
            -> getBool( Path{ "engine", "trap" }, true )
        );
    }

    getLog()
    -> trapOn()
    -> begin( "Loop" )
    -> lineEnd();

    /* Begin of monitoring */
    getMon()
    -> startTimer( Path{ "momentMcs" })
    -> interval( Path{ "uptime" }, Path{ "momentMcs" }, Path{ "startMks" })
    -> setDouble( Path{ "fps" }, getFps() )
    -> addInt( Path{ "count" })
    ;

    if( isOk() )
    {
        /* Check enabled */
        auto enabled = getApplication()
        -> getConfig()
        -> getBool( Path{ "engine", "enabled" }, true );

        getMon() -> setBool( Path{ "enabled" }, enabled );

        if( !enabled )
        {
            setCode( "disabled" );
        }

        onEngineLoop( configUpdated, enabled );
    }

    /*
        Define result state action
    */
    auto code = getApplication()
    -> getConfig()
    -> getObject( Path{ "engine", getId(), "code", getCode() });

    if( code == NULL )
    {
        /* Read default result state action */
        code = getApplication()
        -> getConfig()
        -> getObject( Path{ "engine", getId(), "code", "*" });
    }

    if( code != NULL )
    {
        /* Log out */
        getLog()
        -> record
        (
            Log::logRecordFromString( code -> getString( Path{ "log" }, "ERROR" )),
            getCode()
        )
        -> dump( getDetails(), "Details" )
        -> text( getMessage() );

        /* Exit form payload */
        if( code -> getBool( Path{ "exit" }, false ))
        {
            stop();
        }

        /* Sleep timeout */
        auto sleep = code -> getInt( Path{ "timeoutMcs" }, 0 );
        if( sleep != 0)
        {
            setLoopTimeoutMcs( sleep );
        }
    }
    else
    {
        getLog() -> error( "unknown_action" ) -> prm( "code", getCode() );
        stop();
    }


    /* Final monitoring */
    getMon() -> setString( Path{ "Result" }, getCode() ) -> flush();
    getLog()
    -> end()
    -> trapOff()
    ;

    /* fps processing */
    auto current = now();

    if( fpsLast != 0 )
    {
        fps = SECOND / (double)( current - fpsLast );
    }

    fpsLast = current;
}



/*
    Payload engine loop default event
*/
void PayloadEngine::onEngineLoop
(
    /* true if application config updated */
    const bool,
    /* true for enabled service */
    const bool
)
{
    /* Can be overrided in childrens */
}



double PayloadEngine::getFps()
{
    return fps;
}
