#pragma once

#include <stddef.h>


/*
    Chain item empty parent class
    This class can be using like prototype for other classes with values
*/

class ChainItem
{
    private:

        ChainItem*  prev    = NULL;
        ChainItem*  next    = NULL;

    public:


        virtual ~ChainItem();



        /*
            Create new item
        */
        static ChainItem* create();



        /*
            Destroy the item
        */
        void destroy();



        /*
            Return the next item or NULL for last item
        */
        ChainItem* getNext();



        /*
            Return the preveious item or NULL for the first item
        */
        ChainItem* getPrev();



        /*
            Set next item for current item
            WARNING. Do not use this method.
        */
        ChainItem* setNext
        (
            ChainItem* /* The next item  */
        );



        /*
            Set prev item for current item
            WARNING. Do not use this method.
        */
        ChainItem* setPrev
        (
            ChainItem*  /* The previous item */
        );
};
