#include "chart_list.h"




/*
    Create new item
*/
ChartList* ChartList::create()
{
    return new ChartList();
}



/*
    Destroy the object
*/
void ChartList::destroy()
{
    delete this;
}



/*
    Push new element
*/
ChartData* ChartList::add
(
    string aId
)
{
    auto result = byId( aId );
    if( result == NULL )
    {
        result = ChartData::create();
        list.insert({ aId, result });
    }
    return result;
}



/*
    Push new element
*/
ChartData* ChartList::byId
(
    string aId
)
{
    if( list.find( aId ) != list.end())
    {
        return list[ aId ];
    }
    return NULL;
}
