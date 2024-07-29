#pragma once

#include "sys/time.h"
#include <string>



using namespace std;



class RndObj
{
    private:

        unsigned long long seed;         /* Current seed */
        unsigned long long storedSeed;   /* Stored seed */

    public:
        /*
            Constructor that initializes the seed
        */
        RndObj
        (
            /* initial seed value */
            unsigned long long
        );



        /*
            Static method to create an instance and return a pointer to it
        */
        static RndObj* create
        (
            /* initial seed value */
            unsigned long long
        );



        /*
            Object selfdestructor
        */
        void destroy();



        /*
            Begin randomize from current time
        */
        RndObj* randomize();



        /*
            Store seed value
        */
        RndObj* storeSeed
        (
            unsigned long long
        );



        /*
            Restore seed value
        */
        RndObj* restoreSeed();



        /*
            Get random value
        */
        RndObj* calcSeed();



        /*
            Get random value
        */
        double get();



        /*
            Get random value from AMin to AMax
        */
        int get
        (
            /* minimum value */
            int,
            /* maximum value */
            int
        );



        /*
            Get random value from AMin to AMax
        */
        unsigned int get
        (
            /* minimum value */
            unsigned int,
            /* maximum value */
            unsigned int
        );



        /*
            Get random value from AMin=0 to AMax=0xFFFFFFFFFFFFFFFF
        */
        unsigned long long get
        (
            /* minimum value */
            unsigned long long,
            /* maximum value */
            unsigned long long
        );



        /*
            Get random value from AMin to AMax
        */
        double get
        (
            /* minimum value */
            double,
            /* maximum value */
            double
        );



        /*
            Return uuid
        */
        string getUuid();



        /*
            Return uuid by seed
        */
        string getUuid
        (
            /* seed for current UUID */
            unsigned long long
        );



        /*
            Return whether a probabilistic event should occur
        */
        bool chanceByTime
        (
            /* time interval */
            long,
            /* frequency */
            long
        );
};
