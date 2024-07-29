#include <cmath>

#include "rnd_obj.h"



/*
    Constructor that initializes the seed
*/
RndObj::RndObj
(
    /* initial seed value */
    unsigned long long aInitialSeed
)
{
    seed = aInitialSeed;
}



/*
    Static method to create an instance and return a pointer to it
*/
RndObj* RndObj::create
(
    /* initial seed value */
    unsigned long long aInitialSeed
)
{
    return new RndObj( aInitialSeed );
}



/*
    Object selfdestructor
*/
void RndObj::destroy()
{
    delete this;
}



/*
    Begin randomize from current time
*/
RndObj* RndObj::randomize()
{
    timeval currentTime;
    gettimeofday( &currentTime, 0 );
    seed = ( currentTime.tv_usec );

    return this;
}



/*
    Store seed
*/
RndObj* RndObj::storeSeed
(
    unsigned long long aSeed
)
{
    storedSeed = seed;
    seed = aSeed;

    return this;
}



RndObj* RndObj::restoreSeed()
{
    seed = storedSeed;

    return this;
}



/*
    Get random value
*/
RndObj* RndObj::calcSeed()
{
    seed = (seed * 71103515245 + 17344371) & 0xFFFFFFFFFFFF;
    return this;
}



/*
    Get random value
*/
double RndObj::get()
{
    calcSeed();
    return (double) seed / (double) 0x1000000000000;
}



/*
    Get random value from AMin to AMax
*/
int RndObj::get
(
    int aMin,
    int aMax
)
{
    return aMin + floor( get() * ( aMax + 1  - aMin ));
}




/*
    Get random value from AMin to AMax
*/
unsigned int RndObj::get
(
    unsigned int aMin,
    unsigned int aMax
)
{
    return aMin + floor( get() * ( aMax + 1  - aMin ));
}



/*
    Get random value from AMin=0 to AMax=0xFFFF FFFF FFFF
*/
unsigned long long RndObj::get
(
    unsigned long long aMin,
    unsigned long long aMax
)
{
    return aMin + floor( get() * ( aMax + 1  - aMin ));
}



/*
    Get random value from AMin to AMax
*/
double RndObj::get
(
    double aMin,
    double aMax
)
{
    return aMin + get() * ( aMax - aMin );
}



/*
    Return uuid
*/
string RndObj::getUuid()
{
    char buffer[37];
    sprintf(buffer, "%08x-%04x-%04x-%04x-%04x%08x",
            get((unsigned int)0, (unsigned int)0xFFFFFFFF),
            get(0, 0xFFFF),
            ((get(0, 0xFFFF) & 0x0fff) | 0x4000),
            get(0, 0xFFFF) % 0x3fff + 0x8000,
            get((unsigned int) 0, (unsigned int) 0xFFFF),
            get((unsigned int) 0, (unsigned int) 0xFFFFFFFF));
            string result( buffer );
    return result;
}



/*
    Return uuid by seed
*/
string RndObj::getUuid
(
    unsigned long long aSeed    /* Seed for current UUID */
)
{
    string result;
    storeSeed( aSeed );
    result = getUuid();
    restoreSeed();
    return result;
}



/*
    возвращает необходимость исолнения вероятного события
*/
bool RndObj::chanceByTime
(
    long aInterval,
    long aFrequancy
)
{
    double r = get();
    double c = (double) aFrequancy / (double) aInterval;
    return r < c;
}

