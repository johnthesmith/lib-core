#pragma once


#include "result.h"


class BufferD1 : public Result
{
    private:

        double* buffer;
        long long int count;

    public:

        /*
            The buffer constructor
        */
        BufferD1
        (
            long long int = 0   /* Count of items */
        );



        /*
            Buffer destuctor
        */
        ~BufferD1();



        /*
            Create new buffer
        */
        static BufferD1* create
        (
            long long int = 0   /* Count of items */
        );



        /* Buffer destroy */
        void destroy();



        /*
            Set value by index
        */
        BufferD1* setValue
        (
            long long int,  /* Index of item */
            double          /* Value */
        );



        /*
            Get item by index
        */
        double getValue
        (
            long long int   /* Index of valu*/
        );



        /*
            Return buffer address
        */
        double* getBuffer();



        /*
            Return count of items
        */
        BufferD1* setCount
        (
            long long int
        );



        /*
            Return count of items
        */
        long long int getCount();



        /*
            Return memory buffer
        */
        char* getMemBuffer();



        /*
            Return size of bufer
        */
        size_t getMemSize();
};
