#include <iostream>
#include <cstring>
#include "heap.h"

using namespace std;

/*
    [Heap] ---> [ void*; void* ; void* ; .... ]
*/



Heap::Heap()
{
}



/*
    Destructor
*/
Heap::~Heap()
{
    resize( 0 );
}



/*
    Create and return heap
*/
Heap* Heap::create()
{
    return new Heap();
}



/*
    Destroy heap
*/
void Heap::destroy()
{
    delete( this );
}



Heap* Heap::setItems
(
    void ** a
)
{
    if( items != NULL )
    {
        delete [] items;
    }
    items = a;
    return this;
}



/*
    Add heap from argument to this heap
*/
Heap* Heap::add
(
    Heap* a
)
{
    auto currentSize = count;
    auto sourceSize = a -> count;
    auto newSize = currentSize + sourceSize;

    resize( newSize );

    memcpy
    (
        &items[ currentSize ],
        a -> items,
        sourceSize * sizeof( void* )
    );

    return this;
}




/*
    Return index by neuron
*/
int Heap::indexBy
(
    void* a
)
{
    int result = -1;
    for( int i = 0; i < count && result == -1; i++ )
    {
        if( a == items[ i ] )
        {
            result = i;
        }
    }
    return result;
}




/*
    Return index by neuron
*/
void* Heap::getByIndex
(
    int a
)
{
    if( a >= 0 && a < count )
    {
        return items[ a ];
    }
    else
    {
        return NULL;
    }
}




/*
    Set value by index
*/
Heap* Heap::setByIndex
(
    int aIndex,
    void* aNeuron
)
{
    if( aIndex >= 0 && aIndex < count )
    {
        items[ aIndex ] = aNeuron;
    }
    return this;
}



/*
    Resize
*/
Heap* Heap::resize
(
    int a
)
{
    /* Get current size */
    auto currentSize = count;

    if( a != currentSize )
    {
        /* Create new element */
        void** newItems = a == 0 ? NULL : new void*[ a ];

        if( items != NULL && newItems != NULL )
        {
            /* Copy */
            memcpy
            (
                newItems,
                items,
                min( currentSize, a ) * sizeof( void* )
            );
        }

        /* Set items to new items */
        setItems( newItems );

        /* Sct count */
        count = a;
    }
    return this;
}



/*
    Resize
*/
Heap* Heap::expand
(
    int a
)
{
    resize( count + a );
    return this;
}



/*
    Push one elemen
*/
Heap* Heap::push
(
    void* a
)
{
    expand( 1 );
    items[ count - 1 ] = a;
    return this;
}



/*
    Remove one element and resize object
*/
void* Heap::remove
(
    int a
)
{
    void* result = NULL;
    if( a >= 0 && a < count )
    {
        result = items[ a ];

        /* Create new elements */
        void** newItems = count == 1 ? NULL : new void*[ count - 1 ];

        if( newItems != NULL )
        {
            /* Copy before */
            memcpy
            (
                newItems,
                items,
                a * sizeof( void* )
            );

            if( a != count - 1 )
            {
                /* Copy after */
                memcpy
                (
                    &newItems[ a ], /* Destination */
                    &items[ a + 1],
                    ( count - 1 - a ) * sizeof( void* )
                );
            }

            /* Set items to new items */
            setItems( newItems );

            count--;
        }
        else
        {
            resize();
        }
    }
    return result;
}



/*
    Remove one element and resize object
*/
void* Heap::remove
(
    void* a
)
{
    return remove( indexBy( a ));
}



///*
//    Remove all elements form  this,
//    contains in the argument
//*/
//Heap* Heap::remove
//(
//    Heap* aRemove
//)
//{
////    remove
////    (
////        [ &aRemove ]
////        ( void* aItem )
////        {
////            return aRemove -> indexBy( aItem ) >= 0;
////        }
////    );
//    return this;
//}



/*
    Remove some elememnts from start index

                count=5
                <=5=>
    Source: 0123456789
                |
                index=4

    Result: 01239

*/
Heap* Heap::bite
(
    int aIndex, /* Strat index */
    int aCount  /* Count for bite */
)
{
    if
    (
        aIndex >= 0 &&
        aIndex + aCount <= count
    )
    {
        /* Create new element */
        void** newItems = aCount == count ? NULL : new void*[ count - aCount ];

        if( newItems != NULL )
        {
            /* Copy before */
            memcpy
            (
                newItems,
                items,
                aIndex * sizeof( void* )
            );

            if( aIndex + aCount < count )
            {
                /* Copy after */
                memcpy
                (
                    &newItems[ aIndex ],
                    &items[ aIndex + aCount ],
                    ( count - ( aIndex + aCount ) ) * sizeof( void* )
                );
            }

            /* Set items to new items */
            setItems( newItems );

            /* Set actual size */
            count = count - aCount;
        }
        else
        {
            resize();
        }
    }

    return this;
}



/*
    Return first element or null if not exists
*/
void* Heap::getFirst()
{
    return getCount() > 0 ? getByIndex( 0 ) : NULL;
}




/*
    Merge two heaps
    Each elemento of Argument heap will add to This heap
    if it not exists in This
*/
Heap* Heap::merge
(
    Heap* a
)
{
    return merge
    (
        a,
        [ this ]
        ( void* aItem )
        {
            return indexBy( aItem ) < 0;
        }
    );
}
