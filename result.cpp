#include <iostream>
#include <string>

#include "result.h"
#include "../json/param_list.h"

using namespace std; // для того что бы std:: не приходилось каждый раз писать



/*
    Constructor
*/
Result::Result()
{
    details = ParamList::create();
}



/*
    Destructor
*/
Result::~Result()
{
    details -> destroy();
}




Result*  Result::create
(
    string aCode,
    string aMessage
)
{
    auto result = new Result();
    return result -> setCode( aCode ) -> setMessage( aMessage );
}




void Result::destroy()
{
    delete this;
}




Result* Result::setOk()
{
    code = RESULT_OK;
    return this;
}



Result* Result::setCode
(
    string a  /* String for Text */
)
{
    code = a;
    return this;
}



string Result::getCode()
{
    return code;
}



Result* Result::setMessage
(
    string a  /* String for Text */
)
{
    message = a;
    return this;
}



string Result::getMessage()
{
    return message;
}



/*
    Set result and message
*/
Result* Result::setResult
(
    string aCode,       /* Code value */
    string aMessage     /* Message value */
)
{
    setCode( aCode );
    setMessage( aMessage );
    return this;
}



bool Result::isOk()
{
    return code == RESULT_OK;
}



/*
    Set result and message
*/
Result* Result::resultFrom
(
    Result* a
)
{
    setCode( a -> code );
    setMessage( a -> message );
    getDetails() -> copyFrom( a -> getDetails() );
    return this;
}



/*
    Set result and message
*/
Result* Result::resultTo
(
    Result* a
)
{
    a -> resultFrom( this );
    return this;
}



/*
    Details return
*/
ParamList* Result::getDetails()
{
    return details;
}



/**********************************************************************
    Limb synchronization
*/



/*
    Lock Limb for operations with layers
*/
bool Result::lock
(
    bool aSkip
)
{
    /* Let default result */
    bool result = true;
    if( aSkip )
    {
        result = sync.try_lock();
    }
    else
    {
        timeval lockStart, lockEnd;
        gettimeofday(&lockStart, NULL);
        sync.lock();
        gettimeofday(&lockEnd, NULL);

        lockTime +=
        (( unsigned long long int ) ( lockEnd.tv_sec - lockStart.tv_sec )) * 1000000
        + ( lockEnd.tv_usec - lockStart.tv_usec );
    }
    return result;
}



/*
    Unlock Limb after lock method
*/
Result* Result::unlock()
{
    sync.unlock();
    return this;
}



/*
    Return true for locked
*/
bool Result::isLock()
{
    /* Let default result */
    bool result = !sync.try_lock();
    if( !result )
    {
        sync.unlock();
    }

    return result;
}
