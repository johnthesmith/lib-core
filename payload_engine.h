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
        virtual void internalLoop1();
};
