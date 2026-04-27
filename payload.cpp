/* Default libraries */

#include <sys/syscall.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h> /* sdf */
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <dlfcn.h>

/* Local libraries */
#include "payload.h"


using namespace std;



/*
    Constructor of paylod
*/
Payload::Payload
(
    Application* aApplication,
    string aId
)
{
    application = aApplication;
    config = ParamList::create();
    setId( aId );
    getLog() -> trace( "Payload created" ) -> prm( "id", aId);
}



/*
    Destructor of payload
*/
Payload::~Payload()
{
    if( threadObject != NULL )
    {
        waitStop();
    }
    config -> destroy();
    getLog() -> trace( "Payload destroyed" )  -> prm( "id", id);
}



/*
    Creator
*/
Payload* Payload::create
(
    Application* aApplication,
    string aId
)
{
    return new Payload( aApplication, aId );
}



/*
    Destructor
*/
void Payload::destroy()
{
    delete this;
}




Payload* Payload::load
(
    const string& libraryPath,
    Application* app,
    std::string id
)
{
    Payload* result = nullptr;
    void* handle = dlopen( libraryPath.c_str(), RTLD_NOW );
    if( handle )
    {
        using CreateFunc = Payload* (*)( Application*, std::string );
        CreateFunc create = (CreateFunc) dlsym ( handle, "create_payload" );

        if( create )
        {
            result = (create)( app, id );
        }
        else
        {
            dlclose( handle );
        }

    }
    return result;
}


/******************************************************************************
    Control actions
*/



/*
    Main loop
*/
Payload* Payload::start
(
    bool aThread    /* True for run like thread */
)
{
    /*
        Internal loop emplimentation
    */
    auto doLoop = [ this ]()
    {
        onStartAfter();
        while( state == STATE_LOOP )
        {
            internalLoop1();
            if( idling && loopTimeoutMcs != 0 )
            {
                usleep( loopTimeoutMcs );
            }
        }
        onStopAfter();
        state = STATE_STOP;
    };

    if( lock( true ))
    {
        onStartBefore();

        if( aThread )
        {
            /* Run loop in the personal thread if it does not early */
            if( threadObject == NULL )
            {
                state = STATE_LOOP;
                threadObject = new thread
                (
                    [ this, doLoop ]
                    ()
                    {
                        /* Log create and registration */
                        application -> createThreadLog( getId() );
                        /* Run loop */
                        doLoop();
                        /* Destroy and nullate log */
                        application -> onThreadAfter();
                        application -> destroyThreadLog();
                    }
                );
            }
        }
        else
        {
            /* Run loop in the parent thread */
            state = STATE_LOOP;
            /* Run loop */
            doLoop();
        }

        unlock();
    }

    return this;
}



/*
    Send stop state for thread
*/
Payload* Payload::stop()
{
    if( state == STATE_LOOP )
    {
        state = STATE_WAIT_STOP;
        onStopBefore();
    }
    return this;
}




/*
    Wait stop thread
*/
Payload* Payload::waitStop()
{
    if( lock( true ))
    {
        if( threadObject != NULL )
        {
            getLog()
            -> begin( "Thread stop waiting" )
            -> prm( "id", getId() )
            -> lineEnd();

            if( state != STATE_STOP )
            {
                stop();
                while( state == STATE_WAIT_STOP )
                {
                    usleep( 1000 );
                };
            }
            threadObject -> join();
            delete threadObject;
            threadObject = NULL;
            getLog() -> end() -> lineEnd();
        }
        unlock();
    }
    return this;
}





/*
    Internal loop emplimentation
    This method calls a user onLoop
*/
void Payload::internalLoop1()
{
    onLoop();
}




/*
    Set idling mode
*/
Payload* Payload::setIdling
(
    bool a   /* Value */
)
{
    idling = a;
    return this;
}




/*
    Set loop time out at microseconds
*/
Payload* Payload::setLoopTimeoutMcs
(
    unsigned int a    /* Value */
)
{
    loopTimeoutMcs = a;
    return this;
}



/*
   Rreturn loop time out at microseconds
*/
unsigned int Payload::getLoopTimeoutMcs()
{
    return loopTimeoutMcs;
}




/******************************************************************************
    Events
*/


/*
    Payload loop before default even
*/
void Payload::onLoop()
{
    getLog() -> trace( "Payload loop default event" );
}



/*
    Payload loop before default even
*/
void Payload::onStartBefore()
{
    getLog() -> trace( "Payload loop before default event" );
}



/*
    Payload loop after default event
*/
void Payload::onStartAfter()
{
    getLog() -> trace( "Payload loop after default event" );
}



/*
    On stop before event
*/
void Payload::onStopBefore()
{
    getLog() -> trace( "Process stopping begin" );
}



/*
    On pause event when process paused
*/
void Payload::onStopAfter()
{
    getLog() -> trace( "Process stoped" );
}




/******************************************************************************
    Setters and getters
*/



/*
    Get application log or personal payloads log if exists
*/
Log* Payload::getLog()
{
    return application -> getLog();
}



/*
    Get application monitor object
*/
Mon* Payload::getMon()
{
    return application -> getMon();
}



/*
    Get application object
*/
Application* Payload::getApplication()
{
    return application;
}



/*
    Set payload id
*/
Payload* Payload::setId
(
    string aId
)
{
    id = aId;
    return this;
}




ThreadState Payload::getState()
{
    return state;
}




/*
    Convert ThreadState to string
*/
string stateToString
(
    ThreadState a
)
{
    switch( a )
    {
        case STATE_WAIT_STOP: return "WAIT_STOP";
        case STATE_STOP: return "STOP";
        case STATE_LOOP: return "LOOP";
        default: return "UNKNOWN";
    }
}
