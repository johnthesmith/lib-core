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
    public:

        /*
            Constructor
        */
        PayloadEngine
        (
            Application*
        );



        virtual ~PayloadEngine();



        /*
            Creator
        */
        static PayloadEngine* create
        (
            Application*
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
        virtual void onLoopBefore();



        /*
            EnginePayload loop before default even
        */
        virtual void onEngineLoopBefore();

};
