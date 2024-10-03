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
        getLog() -> trace( "Destroying payload thread" ) -> prm( "id", id );
        stop();
        waitStop();
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




/******************************************************************************
    Control actions
*/



/*
    Mail loop
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
        state = STATE_LOOP;
        onStartBefore();
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
                onStartAfter();
            }
        }
        else
        {
            /* Run loop in the parent thread */
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
        if( threadObject != NULL && state == STATE_WAIT_STOP )
        {
            getLog()
            -> begin( "Thread stop waiting" )
            -> prm( "id", id )
            -> lineEnd();

            while( state == STATE_WAIT_STOP )
            {
                usleep( 1000 );
            };

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
