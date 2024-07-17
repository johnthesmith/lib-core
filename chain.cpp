#include <iostream>
#include "chain.h"



/*
    Create new item
*/
Chain* Chain::create()
{
    return new Chain();
}



/*
    Destroy the object
*/
void  Chain::destroy()
{
    clear();
    delete this;
}



/*
    Delete all elements
*/
Chain* Chain::clear()
{
    ChainItem* current = first;
    while( current != NULL )
    {
        auto clearing = current;
        current = current -> getNext();
        remove( clearing );
        clearing -> destroy();
    }
    return this;
}



/*
    Insert element before argument
    A B  *D* E
        C
*/
Chain* Chain::insertBefore
(
    ChainItem* aItem,      /* Inserting item */
    ChainItem* aBefore     /* Insert before item */
)
{
    if( aItem -> getPrev() == NULL && aItem -> getPrev() == NULL )
    {
        if( aItem != NULL && aBefore != NULL )
        {
            auto prev = aBefore -> getPrev();

            if( prev != NULL )
            {
                prev -> setNext( aItem );
                aItem -> setPrev( prev );
            }

            aItem -> setNext( aBefore );
            aBefore -> setPrev( aItem );

            if( aBefore == getFirst() )
            {
                first = aItem;
            }

            count++;
        }
        else
        {
            setCode( "item_or_bevore_is_null" );
        }
    }
    else
    {
        setCode( "item_alrady_has_neighbours" );
    }
    return this;
}



/*
    Create new element before argument
    A B  *D* E
        C
*/
ChainItem* Chain::createBefore
(
    ChainItem* aBefore
)
{
    ChainItem* result = NULL;
    if( aBefore != NULL )
    {
        /* create new item */
        result = ChainItem::create();
        insertBefore( result, aBefore );
    }
    return result;
}




/*
    Insert element after argument
    A B *D* E
           C
*/
Chain* Chain::insertAfter
(
    ChainItem* aItem,      /* Inserting item */
    ChainItem* aAfter      /* Insert after item */
)
{
    if( aItem -> getPrev() == NULL && aItem -> getPrev() == NULL )
    {
        if( aItem != NULL && aAfter != NULL )
        {
            auto next = aAfter -> getNext();
            if( next != NULL )
            {
                next -> setPrev( aItem );
                aItem -> setNext( next );
            }
            aItem -> setPrev( aAfter );
            aAfter -> setNext( aItem );

            if( aAfter == getLast() )
            {
                last = aItem;
            }

            count++;
        }
        else
        {
            setCode( "item_or_after_is_null" );
        }
    }
    else
    {
        setCode( "item_alrady_has_neighbours" );
    }
    return this;
}



/*
    Create new element after the argument
    A B  *D* E
        C
*/
ChainItem* Chain::createAfter
(
    ChainItem* aAfter
)
{
    ChainItem* result = NULL;
    if( aAfter != NULL )
    {
        /* create new item */
        result = ChainItem::create();
        insertAfter( result, aAfter );
    }
    return result;
}



/*
    Insert the argument in to back of list
*/
Chain* Chain::insertLast
(
    ChainItem* aItem
)
{
    if( count == 0 )
    {
        last = aItem;
        first = aItem;
        count++;
    }
    else
    {
        insertAfter( aItem, getLast() );
    }
    return this;
}




/*
    Create new last element
*/
ChainItem* Chain::createLast()
{
    auto result = ChainItem::create();
    insertLast( result );
    return result;
}




/*
    Insert the argument in to a head of list
*/
Chain* Chain::insertFirst
(
    ChainItem* aItem
)
{
    if( count == 0 )
    {
        last = aItem;
        first = aItem;
        count++;
    }
    else
    {
        insertBefore( aItem, getFirst() );
    }
    return this;
}



/*
    Create new first element
*/
ChainItem* Chain::createFirst()
{
    auto result = ChainItem::create();
    insertFirst( result );
    return result;
}



/*
    Remove item from linked list
*/
Chain* Chain::remove
(
    ChainItem* aItem
)
{
    if( count > 0 )
    {
        auto prev = aItem -> getPrev();
        auto next = aItem -> getNext();

        if( prev != NULL )
        {
            prev -> setNext( next );
        }
        else
        {
            first = next;
        }

        if( next != NULL )
        {
            next -> setPrev( prev );
        }
        else
        {
            last = prev;
        }

        aItem -> setNext( NULL );
        aItem -> setPrev( NULL );

        count--;
    }

    return this;
}




/*
    Remove first
*/
Chain* Chain::deleteFirst()
{
    auto removing = first;
    if( removing != NULL )
    {
        remove( removing );
        removing -> destroy();
    }
    return this;
}



/*
    Remove last
*/
Chain* Chain::deleteLast()
{
    auto removing = last;
    if( removing != NULL )
    {
        remove( removing );
        removing -> destroy();
    }
    return this;
}



/*
    Loop back
*/
Chain* Chain::loopBack
(
    function <bool ( ChainItem* )> aCallback
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
}



/*
    Loop front
*/
Chain* Chain::loopFront
(
    function <bool ( ChainItem* )> aCallback,
    ChainItem* aStart
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
}



/*
    Setters and getters
*/

/*
    Return count of elements
*/
unsigned long long int Chain::getCount()
{
    return count;
}



/*
    Return first element
*/
ChainItem* Chain::getFirst()
{
    return first;
}



/*
    Return last element
*/
ChainItem* Chain::getLast()
{
    return last;
}



Chain* Chain::dump
(
    function <string ( ChainItem* )> aCallback
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
}
