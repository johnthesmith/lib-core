/*
    This is a double memory buffer
    Contains the methods for access
*/
#pragma once



#include "result.h"



class BufferD1 : public Result
{
    private:

        /* Buffer memory pointer */
        double* buffer = NULL;
        /* Size of buffer */
        long long int count = 0;
        /*
            Buffer is foreign
            true - buffer is foreign, exernal memory buffer is using
            false - buffer is private (create and destroy by this object)
        */
        bool foreign = false;

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
            Set buffer from memory
            Buffer have to empty
        */
        BufferD1* setMem
        (
            char* aMemBuffer,
            size_t aMemSize
        );



        /*
            Return memory buffer
        */
        char* getMemBuffer();



        /*
            Return size of bufer
        */
        size_t getMemSize();



        /*
            Dump to strout
        */
        BufferD1* dump
        (
            unsigned long long int = 0,
            unsigned long long int = 0
        );
};
