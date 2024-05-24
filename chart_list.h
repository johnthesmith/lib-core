#pragma once


#include "chart_data.h"
#include <map>


class ChartList
{
    private:
        map <string, ChartData*> list;
    public:

        /*
            Create new item
        */
        static ChartList* create();



        /*
            Destroy the object
        */
        void destroy();



        /*
            Push new element
        */
        ChartData* add
        (
            string /* id */
        );



        /*
            Push new element
        */
        ChartData* byId
        (
            string /* id */
        );
};
