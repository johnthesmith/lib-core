#include "chain_item.h"




ChainItem::~ChainItem()
{
}



/*
    Create new item
*/
ChainItem* ChainItem::create()
{
    return new ChainItem();
}



/*
    Destroy the item
*/
void ChainItem::destroy()
{
    delete this;
}



/*
    Return the next item or NULL for last item
*/
ChainItem* ChainItem::getNext()
{
    return next;
}



/*
    Return the preveious item or NULL for the first item
*/
ChainItem* ChainItem::getPrev()
{
    return prev;
}



/*
    Set next item for current item
    WARNING. Do not use this method.
*/
ChainItem* ChainItem::setNext
(
    ChainItem*  a /* The next item  */
)
{
    next = a;
    return this;
}



/*
    Set prev item for current item
    WARNING. Do not use this method.
*/
ChainItem* ChainItem::setPrev
(
    ChainItem*  a /* The previous item */
)
{
    prev = a;
    return this;
}


