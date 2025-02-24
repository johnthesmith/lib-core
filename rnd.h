#pragma once

#include "sys/time.h"
#include <string>



using namespace std;



class Rnd
{
    private:

        unsigned long long seed = 0;            /* Current seed */

    public:

        static Rnd* create();
        void destroy();



        /*
            Begin randomize from current time
        */
        Rnd* randomize();



        /*
            Change seed directly
        */
        Rnd* setSeed
        (
            unsigned long long int
        );




        /*
            Return current seed directly
        */
        unsigned long long int getSeed();



        /*
            Get random value
        */
        double get();



        /*
            Get random value from AMin to AMax
        */
        int get
        (
            /* Minimum */
            int,
            /* Maximum */
            int
        );



        /*
            Get random value from AMin to AMax
        */
        unsigned int get
        (
            /* Minimum */
            unsigned int,
            /* Maximum */
            unsigned int
        );



        /*
            Get random value from AMin=0 to AMax=0xFFFF FFFF FFFF
        */
        unsigned long long get
        (
            unsigned long long,
            unsigned long long
        );



        /*
            Get random value from AMin to AMax
        */
        double get
        (
            double,
            double
        );



        /*
            Return uuid
        */
        string getUuid();



        /*
            возвращает необходимость исолнения вероятного события
        */
        bool chanceByTime
        (
            long,
            long
        );
};
