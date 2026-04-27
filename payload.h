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
        Application*    application         = nullptr;
        thread*         threadObject        = nullptr;
        /* Config of payload, section of application config */
        ParamList*      config              = nullptr;
        long int        lastConfigUpdate    = 0;

        /* States */
        unsigned int    loopTimeoutMcs      = 0;
        bool            idling              = true;
        string          id                  = "";
        ThreadState     state               = STATE_STOP;

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
            /* Application object */
            Application*,
            /* Id of payload */
            string
        );



        virtual ~Payload();



        /*
            Creator
        */
        static Payload* create
        (
            Application*,
            string
        );



        /*
            Destructor
        */
        void destroy();



        static Payload* load
        (
            const string& libraryPath,
            /* Application object */
            Application* app,
            /* Payload id */
            std::string
        );

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
            Return the id of payload
        */
        inline string getId()
        {
            return id;
        }




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



        /*
            Return payload config
        */
        ParamList* getConfig()
        {
            return config;
        }



        /*
            Return true if application confgi was updated
        */
        bool isConfigUpdate()
        {
            return
            getApplication() -> getLastConfigUpdate()
            != getLastConfigUpdate();
        }



        Payload* updateConfig()
        {
            config -> clear();

            auto cfg = getApplication() -> getConfig();
            if( cfg -> lock())
            {
                cfg -> resultTo( this );

                auto section = cfg
                -> getObject( Path{ "engine", "payloads", getId(), "config" });
                if( section != nullptr )
                {
                    config -> copyFrom( section );
                }

                lastConfigUpdate = getApplication() -> getLastConfigUpdate();

                cfg -> unlock();
            }

            return this;
        }



        bool checkConfig()
        {
            auto result = isConfigUpdate();
            if( result )
            {
                updateConfig();
            }

            if( result )
            {
                getLog() -> setTrapEnabled
                (
                    getConfig() -> getBool( Path{ "log", "trap" }, true )
                );
            }

            return result;
        }



        /*
            Return get last config update
        */
        inline long int getLastConfigUpdate()
        {
            return lastConfigUpdate;
        }

};



