/*
    Work with moment of time
    2024.03.03
    still@itserv.ru
*/

#pragma once

#include <stdio.h>
#include <time.h>
#include <string>

using namespace std;

const long long MICROSECOND      = 1;
const long long MILLISECOND      = MICROSECOND * 1000;
const long long SECOND           = MILLISECOND * 1000;
const long long MINUTE           = SECOND * 60;
const long long HOUR             = MINUTE * 60;
const long long DAY              = HOUR * 24;
const long long MONTH            = DAY * 30;
const long long YEAR             = DAY * 365;



enum IntervalFormat
{
    /* 0 year 0 month 0 day 0 hour 0 min 0 sec 0 mls 0 mcs */
    INTERVAL_FORMAT_STRING,
    /* 0:0:0 0:0:0.000000 */
    INTERVAL_FROMAT_NUMBER
};



struct Moment
{
    private:

        long long value = 0;

    public :

        /**********************************************************************
            Construction
        */


        /*
            Constructor
        */
        Moment
        (
            long long = 0 /* Default value */
        );



        /*
            Create moment
        */
        static Moment create
        (
            long long = 0 /* Default value */
        );



        /**********************************************************************
            Methods
        */



        /*
            Set now in to value
        */
        Moment& now();



        /*
            Return moment to string
        */
        string toString();



        /*
            Return delta between this moment and arguments
        */
        long long delta
        (
            Moment&
        );



        /*
            Add value
        */
        Moment& add
        (
            long long
        );



        /*
            Return string delta between this moment and arguments
        */
        string intervalToString();



        /**********************************************************************
            Seters and getters
        */



        /*
            Get value of moment
        */
        long long get();



        /*
            Return value of moment
        */
        Moment& set
        (
            long long = 0 /* Value */
        );
};
