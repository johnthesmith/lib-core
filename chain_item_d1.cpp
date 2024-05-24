#include "chain_item_d1.h"


/*
    Create new item
*/
ChainItemD1* ChainItemD1::create()
{
    return new ChainItemD1();
}



/*
    Destroy the item
*/
void ChainItemD1::destroy()
{
    delete this;
}



/*
    Set value
*/
ChainItemD1* ChainItemD1::setValue
(
    double a
)
{
    value = a;
    return this;
}



/*
    Return value
*/
double ChainItemD1::getValue()
{
    return value;
}
