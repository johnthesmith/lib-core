#pragma once

/*
    Pyaload.
    Base for custom modules and controllers

    Overall functionality of users can be encapsulated in a child of this class.
*/



/* Vanilla librarits */
#include <thread>



/* Local libraries */
#include "result.h"
#include "application.h"




using namespace std;



enum ThreadState
{
    STATE_WAIT_STOP,
    STATE_STOP,
    STATE_LOOP
};



/*
    Convert ThreadState to string
*/
string stateToString
(
    ThreadState
);



class Payload : public Result
{
    private:

        /* Application object*/
        Application*    application     = NULL;
        thread*         threadObject    = NULL;


        /* States */
        unsigned int    loopTimeoutMcs  = 0;
        bool            idling          = true;
        string          id              = "";
        ThreadState     state           = STATE_STOP;

        /*
            Set paused confirmation
        */
        Payload* setPaused
        (
            bool
        );


    public:

        /*
            Constructor
        */
        Payload
        (
            Application*
        );



        virtual ~Payload();



        /*
            Creator
        */
        static Payload* create
        (
            Application*
        );



        /*
            Destructor
        */
        void destroy();




        /******************************************************************************
            Control actions
        */

        /*
            Main payload loop start
        */
        Payload* start
        (
            /* True for run like thread */
            bool = false
        );



        /*
            Set terminate flag and stop the thread if exists
        */
        Payload* stop();




        /*
            Wait pause
        */
        Payload* waitStop();




        /******************************************************************************
            Events
        */



        /*
            User emplementaion
            This method must be overriden
        */
        virtual void onLoop();



        /*
            Payload loop before default even
        */
        virtual void onStartBefore();



        /*
            Payload loop after default event
        */
        virtual void onStartAfter();



        /*
            On stop before event
        */
        virtual void onStopBefore();



        /*
            On stop event
        */
        virtual void onStopAfter();




        /******************************************************************************
            Setters and getters
        */

        /*
            Returen applicaiton pointer
        */
        virtual Application* getApplication();


        /*
            Set payload id
        */
        Payload* setId
        (
            string
        );




        /*
            Get scene value
        */
        Log* getLog();



        /*
            Get scene value
        */
        Mon* getMon();



        /*
            Set idling mode
                true - pause will work
                false - pause will be skip
        */
        Payload* setIdling
        (
            bool    /* Value */
        );



        /*
            Set loop time out at microseconds
        */
        Payload* setLoopTimeoutMcs
        (
            unsigned int /* Value */
        );



        /*
            Get loop time out at microseconds
        */
        unsigned int getLoopTimeoutMcs();



        virtual void internalLoop1();



        /*
            Return thread state
        */
        ThreadState getState();
};
