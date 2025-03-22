#include <iostream>
#include <cmath>
#include "rnd.h"


/*
    Constructor
*/
Rnd* Rnd::create()
{
    return new Rnd();
}



/*
    Destructor
*/
void Rnd::destroy()
{
    delete this;
}



/*
    Begin randomize from current time
*/
Rnd* Rnd::randomize()
{
    timeval currentTime;
    gettimeofday( &currentTime, 0 );
    setSeed( currentTime.tv_usec );
    return this;
}



/*
    Store seed
*/
Rnd* Rnd::setSeed
(
    unsigned long long aSeed
)
{
    seed = aSeed;
    return this;
}



unsigned long long int Rnd::getSeed()
{
    return seed;
}



/*
    Get random value
*/
double Rnd::get()
{
    seed = seed * 6364136223846793005ULL + 1;
    return (double)((seed ^ (seed >> 22)) & 0xFFFFFFFFULL) / 4294967295.0;

//    seed = (seed * 6364136223846793005ULL + 1442695040888963407ULL) & 0xFFFFFFFFFFFFFFFFULL;
//    return (double) seed / (double) 0xFFFFFFFFFFFFFFFFULL;

//    seed = (seed * 71103515245 + 17344371) & 0xFFFFFFFFFF;
//    return (double) seed / (double)          0xFFFFFFFFFF;
}



/*
    Get random value from AMin to AMax
*/
int Rnd::get
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
unsigned int Rnd::get
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
unsigned long long Rnd::get
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
double Rnd::get
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
string Rnd::getUuid()
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
    возвращает необходимость исолнения вероятного события
*/
bool Rnd::chanceByTime
(
    long aInterval,
    long aFrequancy
)
{
    double r = get();
    double c = (double) aFrequancy / (double) aInterval;
    return r < c;
}

