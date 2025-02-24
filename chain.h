#pragma once

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
        template <typename Func> Chain* loopBack
        (
            Func aCallback
        )
        {
            auto stop = false;
            auto current = getLast();
            while( !stop && current != NULL )
            {
                stop = aCallback( current );
                current = current -> getPrev();
            }
            return this;
        };



        /*
            Loop front
        */
        template <typename Func> Chain* loopFront
        (
            Func aCallback,
            ChainItem* aStart = NULL
        )
        {
            auto stop = false;
            auto current = aStart == NULL ? getFirst() : aStart;
            while( !stop && current != NULL )
            {
                stop = aCallback( current );
                current = current -> getNext();
            }
            return this;
        };



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




        template <typename Func> Chain* dump
        (
            Func aCallback
        )
        {
            loopFront
            (
                [ &aCallback]
                ( ChainItem* aCurrent )
                {
                    aCurrent -> getPrev() == NULL
                    ? cout << "NULL          "
                    : cout << aCurrent -> getPrev();

                    cout
                    << " < "
                    << aCallback( aCurrent )
                    << " | "
                    << aCurrent
                    << " > ";

                    aCurrent -> getNext() == NULL
                    ? cout << "NULL          "
                    : cout << aCurrent -> getNext();

                    cout << endl;
                    return false;
                }
            );
            return this;
        };
};
