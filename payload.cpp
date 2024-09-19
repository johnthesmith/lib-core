/* Default libraries */
#include <iostream>
#include <unistd.h>
#include <stdio.h> /* sdf */
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/* Local libraries */
#include "payload.h"


using namespace std;



/*
    Constructor of paylod
*/
Payload::Payload
(
    Application* aApplication
)
{
    application = aApplication;
    getLog() -> trace( "Create payload" );
}



/*
    Destructor of payload
*/
Payload::~Payload()
{
    if( threadObject != NULL )
    {
        pause();
        terminate();
        threadObject -> join();
        delete threadObject;
    }
    getLog() -> trace( "Destroy payload" );
}



/*
    Creator
*/
Payload* Payload::create
(
    Application* aApplication
)
{
    return new Payload( aApplication );
}



/*
    Destructor
*/
void Payload::destroy()
{
    delete this;
}



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
    Run payload
*/
Payload* Payload::run
(
    bool aThread    /* True for run like thread */
)
{
    if( aThread )
    {
        /* Run loop in the personal thread if it does not early */
        if( threadObject == NULL )
        {
            threadObject = new thread
            (
                [ this ]
                ()
                {
                    /* Log create and registration */
                    application -> createThreadLog( id );
                    /* Run work */
                    onRun();
                    application -> onThreadAfter();
                    application -> destroyThreadLog();
                }
            );
        }
        else
        {
            application -> getLog() -> warning( "thread already runing" );
        }
    }
    else
    {
        onRun();
    }
    return this;
}



/*
    Mail loop
*/
Payload* Payload::loop
(
    bool aThread    /* True for run like thread */
)
{
    /*
        Internal loop emplimentation
    */
    auto doLoop = [ this ]()
    {
        terminated  = false;
        onLoopBefore();

        while( !terminated )
        {
            /* Confirm work mode */
            if( state == THREAD_STATE_WORK )
            {
                internalLoop1();
            }

            /* Confirm pause processor */
            if( state == THREAD_STATE_WAIT_PAUSE )
            {
                state = THREAD_STATE_PAUSE;
                onPaused();
            }

            if( idling && loopTimeoutMcs != 0 )
            {
                usleep( loopTimeoutMcs );
            }
        }
        onLoopAfter();
    };


    if( aThread )
    {
        /* Run loop in the personal thread if it does not early */
        if( threadObject == NULL )
        {
            threadObject = new thread
            (
                [ this, doLoop ]
                ()
                {
                    /* Log create and registration */
                    application -> createThreadLog( id );
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
        doLoop();
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
    Set terminate flag
*/
Payload* Payload::terminate()
{
    terminated = true;
    return this;
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
void Payload::onLoopBefore()
{
    getLog() -> trace( "Payload loop before default event" );
}



/*
    Payload loop after default event
*/
void Payload::onLoopAfter()
{
    getLog() -> trace( "Payload loop after default event" );
}



/*
    On Run event
*/
void Payload::onRun()
{
    getLog() -> trace( "Run" );
}



/*
    On pause event
*/
void Payload::onPause()
{
    getLog() -> trace( "Process pause begin" );
}



/*
    On pause event when process paused
*/
void Payload::onPaused()
{
    getLog() -> trace( "Process paused" );
}



/*
    On resume event
*/
void Payload::onResume()
{
    getLog() -> trace( "Process resume" );
}



/******************************************************************************
    Setters and getters
*/


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



/******************************************************************************
    Control
*/


/*
    Set pause
*/
Payload* Payload::pause()
{
    if( state == THREAD_STATE_WORK )
    {
        state = THREAD_STATE_WAIT_PAUSE;
        onPause();
    }
    return this;
}



/*
    Continue process after pause
*/
Payload* Payload::resume()
{
    if( state == THREAD_STATE_PAUSE )
    {
        state = THREAD_STATE_WORK;
        onResume();
    }
    return this;
}



/*
    Wait pause
*/
Payload* Payload::waitPause()
{
    if( state == THREAD_STATE_WAIT_PAUSE )
    {
        getLog()
        -> begin( "Thread pause waiting" )
        -> prm( "id", id )
        -> lineEnd();

        while( state == THREAD_STATE_WAIT_PAUSE )
        {
            usleep( 1000 );
        };

        getLog() -> end() -> lineEnd();
    }
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
        case THREAD_STATE_WAIT_PAUSE: return "WAIT_PAUSE";
        case THREAD_STATE_PAUSE: return "PAUSE";
        case THREAD_STATE_WORK: return "WORK";
        default: return "UNKNOWN";
    }
}
