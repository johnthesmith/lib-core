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
    : Payload( application, aId )
{
}



/*
    Destructor of payload
*/
PayloadEngine::~PayloadEngine()
{
}



/*
    Payload loop before default even
*/
void PayloadEngine::onStartBefore()
{
//    getMon() -> now( Path{ "startMks" }, true );
//    getMon() -> now( Path{ "startMoment" }, false );
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
    setOk();

    bool configUpdated = checkConfig();

    getLog()
    -> trapOn()
    -> begin( "Loop" )
    -> prm( "id", getId() )
    -> lineEnd();

//    /* Begin of monitoring */
//    getMon()
//    -> startTimer( Path{ "payloads", getId(), "momentMcs" })
//    -> interval( Path{ "uptime" }, Path{ "momentMcs" }, Path{ "startMks" })
//    -> setDouble( Path{ "fps" }, getFps() )
//    -> addInt( Path{ "count" })
//    ;

    onEngineLoop( configUpdated );


    /*
        Define result state action
    */
    auto code = getConfig() -> getObject( Path{ "code", getCode() });

    if( code == NULL )
    {
        /* Read default result state action */
        code = getConfig() -> getObject( Path{ "code", "*" });
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

        /* Sleep timeout */
        auto sleep = code -> getInt( Path{ "timeoutMcs" }, 0 );
        if( sleep != 0)
        {
            setLoopTimeoutMcs( sleep );
        }
    }
    else
    {
        getLog()
        -> warning( "unknown_action" )
        -> prm( "code", getCode() );
        setLoopTimeoutMcs( 1000000 );
    }

//    /* Final monitoring */
//    getMon()
//    -> setString( Path{ "Result" }, getCode() )
//    -> flush();

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




double PayloadEngine::getFps()
{
    return fps;
}
