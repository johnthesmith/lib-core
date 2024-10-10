#pragma once

/*
    Pyaload Engine.
    Extends the Payload class
*/



/* Local libraries */
#include "payload.h"



using namespace std;




class PayloadEngine : public Payload
{
    private:

        /* Loop frames per second */
        double          fps             = 0;
        long long       fpsLast         = 0;

    public:

        /*
            Constructor
        */
        PayloadEngine
        (
            Application*,
            string
        );



        virtual ~PayloadEngine();



        /*
            Creator
        */
        static PayloadEngine* create
        (
            Application*,
            string
        );



        /*
        */
        virtual void onLoop();



        virtual void onEngineLoop
        (
            /* true if application config updated */
            const bool,
            /* true for enabled service */
            const bool
        );



        /*
            Payload loop before default even
            Does not recoomended for using.
            Use the onEngineLoopBefore
        */
        virtual void onStartBefore();



        /*
            EnginePayload loop before default even
        */
        virtual void onEngineStartBefore();



        double getFps();
};
