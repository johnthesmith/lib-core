#pragma once



#include "chain.h"
#include "buffer_d1.h"
#include "chain_item_d1.h"



class ChainD1 : public Chain
{
    public:

        /*
            Create new item
        */
        static ChainD1* create();



       /*
            Create new element before argument
            A B  *D* E
                C
        */
        ChainItemD1* createBefore
        (
            double,
            ChainItemD1*
        );



        /*
            Create new element after the argument
            A B  *D* E
                C
        */
        ChainItemD1* createAfter
        (
            double,
            ChainItemD1*
        );



        /*
            Insert the argument in to back of list
        */
        ChainItemD1* createLast
        (
            double
        );



        /*
            Insert the argument in to a head of list
        */
        ChainItemD1* createFirst
        (
            double
        );



        /*
            Loop back
        */
        ChainD1* loopBack
        (
            function <bool ( ChainItemD1* )>
        );



        /*
            Loop front
        */
        ChainD1* loopFront
        (
            function <bool ( ChainItemD1* )>
        );



        /*
            Put data in to double buffer
        */
        ChainD1* toBuffer
        (
            BufferD1*
        );
};
