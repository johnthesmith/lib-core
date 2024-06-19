#include "chain_d1.h"




/*
    Create new item
*/
ChainD1* ChainD1::create()
{
    return new ChainD1();
}





/*
    Create new double value before argument
    A B  *D* E
        C
*/
ChainItemD1* ChainD1::createBefore
(
    double aValue,
    ChainItemD1* aBefore
)
{
    ChainItemD1* result = NULL;
    if( aBefore != NULL )
    {
        result = ChainItemD1::create() -> setValue( aValue );
        insertBefore( ( ChainItem* ) result, ( ChainItem* ) aBefore );
    }
    else
    {
        setCode( "before_is_null" );
    }
    return result;
}



/*
    Create new double value after the argument
    A B  *D* E
        C
*/
ChainItemD1* ChainD1::createAfter
(
    double aValue,
    ChainItemD1* aAfter
)
{
    ChainItemD1* result = NULL;
    if( aAfter != NULL )
    {
        result = ChainItemD1::create() -> setValue( aValue );
        insertAfter( ( ChainItem* ) result, ( ChainItem* ) aAfter );
    }
    else
    {
        setCode( "after_is_null" );
    }
    return result;
}



/*
    Create new last element
*/
ChainItemD1* ChainD1::createLast
(
    double aValue
)
{
    auto result = ChainItemD1::create() -> setValue( aValue );
    insertLast( ( ChainItem* ) result );
    return result;
}




/*
    Create new first element
*/
ChainItemD1* ChainD1::createFirst
(
    double aValue
)
{
    auto result = ChainItemD1::create() -> setValue( aValue );
    insertFirst( ( ChainItem* ) result );
    return result;
}



/*
    Loop back
*/
ChainD1* ChainD1::loopBack
(
    function <bool ( ChainItemD1* )> aCallback
)
{
    Chain::loopBack
    (
        [ &aCallback ]
        (
            ChainItem* aItem
        )
        {
            return aCallback( ( ChainItemD1* ) aItem  );
        }
    );
    return this;
}



/*
    Loop front
*/
ChainD1* ChainD1::loopFront
(
    function <bool ( ChainItemD1* )> aCallback
)
{
    Chain::loopFront
    (
        [ &aCallback ]
        (
            ChainItem* aItem
        )
        {
            return aCallback( ( ChainItemD1* ) aItem  );
        }
    );
    return this;
}




/*
    Put data in to double buffer
*/
ChainD1* ChainD1::toBuffer
(
    BufferD1* aBuffer
)
{
    aBuffer -> setCount( getCount() );
    auto i = 0;
    loopFront
    (
        [ &i, &aBuffer ]
        ( ChainItemD1* aItem )
        {
            aBuffer -> setValue( i, aItem -> getValue() );
            i++;
            return false;
        }
    );
    return this;
}




/*
    Put data from double buffer in to chain
*/
ChainD1* ChainD1::fromBuffer
(
    BufferD1* aBuffer
)
{
    /* Clear current */
    clear();
    auto c = aBuffer -> getCount();

    /* Fill the chain */
    for( unsigned long long int i = 0 ; i < c; i++ )
    {
        createLast( aBuffer -> getValue( i ));
    }

    return this;
}




/*
    Put data from memory buffer in to chain
*/
ChainD1* ChainD1::fromBuffer
(
    char* aBuffer,  /* Memory buffer */
    size_t aSize    /* Size of buffer in bytes */
)
{
    auto buffer = BufferD1::create() -> setMem( aBuffer, aSize );
    fromBuffer( buffer);
    buffer -> destroy();

    return this;
}
