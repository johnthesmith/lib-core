#pragma once


#include <functional>   /* for lyambda */

#include "result.h"
#include "chain_item.h"



class Chain : public Result
{
    private:
        /* Count of elements */
        unsigned long long int  count = 0;

        /* First element */
        ChainItem*             first = NULL;

        /* Last element */
        ChainItem*             last = NULL;

    public:

        /*
            Create new item
        */
        static Chain* create();



        /*
            Destroy the object
        */
        void destroy();



        /*
            Delete all elements
        */
        Chain* clear();



        /*
            Insert element before argument
            A B  *D* E
                C
        */
        Chain* insertBefore
        (
            ChainItem*, /* Inserting item */
            ChainItem*  /* Insert before item */
        );



        /*
            Create new element before argument
            A B  *D* E
                C
        */
        ChainItem* createBefore
        (
            ChainItem*
        );



        /*
            Insert element after the argument
            A B  *D* E
                C
        */
        Chain* insertAfter
        (
            ChainItem*, /* Inserting item */
            ChainItem*  /* Insert after item */
        );



        /*
            Create new element after the argument
            A B  *D* E
                C
        */
        ChainItem* createAfter
        (
            ChainItem*
        );



        /*
            Insert the argument in to back of list
        */
        Chain* insertLast
        (
            ChainItem*
        );



        /*
            Create new last element
        */
        ChainItem* createLast();



        /*
            Insert the argument in to a head of list
        */
        Chain* insertFirst
        (
            ChainItem* aItem
        );



        /*
            Create new first element
        */
        ChainItem* createFirst();



        /*
            Remove item from linked list
        */
        Chain* remove
        (
            ChainItem*
        );



        /*
            Remove first
        */
        Chain* deleteFirst();



        /*
            Remove last
        */
        Chain* deleteLast();



        /*
            Loop back
        */
        Chain* loopBack
        (
            function <bool ( ChainItem* )>
        );



        /*
            Loop front
        */
        Chain* loopFront
        (
            function <bool ( ChainItem* )>,
            ChainItem* = NULL
        );



        /*
            Setters and getters
        */

        /*
            Return count of elements
        */
        unsigned long long int getCount();



        /*
            Return first element
        */
        ChainItem* getFirst();


        /*
            Return last element
        */
        ChainItem* getLast();


        Chain* dump
        (
            function <string ( ChainItem* )> aCallback
        );
};
