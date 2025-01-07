#pragma once


#include "chain_d1.h"



class ChartData : public ChainD1
{
    private:
        double  minY = 0.0;
        double  maxY = 1.0;
//        double  minX = 0;
//        double  maxX = 0;
        int     maxCount = 1000;
        bool    autoMinY = true;
        bool    autoMaxY = true;
    public:

        /*
            Create new item
        */
        static ChartData* create();



        /*
            Destroy the object
        */
        void destroy();



        /*
            Push new element
        */
        virtual ChainItemD1* createLast
        (
            double
        );



        /*
            Return chart like string
            > 0123456789 <
             / \
            /   \
           |*****|*****|*****|*****|***...***|
              0     1     2     3      ...9
        */
        string toString
        (
            int
        );



        /*
            Setters and getters
        */


        /*
            Return first element
        */
        double getMaxY();



        /*
            Return first element
        */
        ChartData* setMaxY
        (
            double
        );



        /*
            Return first element
        */
        double getMinY();



        /*
            Return first element
        */
        ChartData* setMinY
        (
            double
        );




        /*
            Return first element
        */
        long long int getMaxCount();



        /*
            Return first element
        */
        ChartData* setMaxCount
        (
            long long int
        );
};
