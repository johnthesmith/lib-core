#pragma once

#include "chain_item.h"


/*
    Queue of double values
*/

class ChainItemD1 : public ChainItem
{
    private:

        double      value   = 0.0;

    public:

        /*
            Create new item
        */
        static ChainItemD1* create();



        /*
            Destroy the item
        */
        void destroy();




        /*
            Set value
        */
        ChainItemD1* setValue
        (
            double
        );



        /*
            Return value
        */
        double getValue();
};
