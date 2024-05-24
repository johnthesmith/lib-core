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
