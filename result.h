#pragma once



#include <string>
#include <mutex>    /* For net sinchronization */

using namespace std;



const string RESULT_OK = "ok";


/*
    Predeclaration
*/
class ParamList;



class Result
{
    private:

        /* Состояния текстового процессора */
        string      message     = "";
        string      code        = RESULT_OK;
        ParamList*  details     = NULL;

        /* Synchronization mutex */
        recursive_mutex sync;

    public:

        /*
            Constructor
        */
        Result();



        /*
            Destructor
        */
        virtual ~Result();



        static Result* create
        (
            string = RESULT_OK,
            string = ""
        );



        /*
            Destroy the object
        */
        virtual void destroy();



        Result* setOk();



        Result* setCode
        (
            string /* String for Text */
        );



        /*
            Return result code
        */
        string getCode();



        /*
            Set message for object
        */
        Result* setMessage
        (
            string /* String for Text */
        );



        /*
            Return message for object
        */
        string getMessage();



        /*
            Set result and message
        */
        Result* setResult
        (
            string,             /* Code value */
            string = ""         /* Message value */
        );



        /*
            Return true if result code is ok
        */
        bool isOk();



        /*
            Copy result from object
        */
        Result* resultFrom
        (
            Result*
        );



        /*
            Copy result to object
        */
        Result* resultTo
        (
            Result*
        );



        /*
            Details return
        */
        ParamList* getDetails();



        /**********************************************************************
            Synchronization
        */

        /*
            Lock Result for operations
        */
        bool lock
        (
            bool = false /* Skip this action for locked mutex */
        );



        /*
            Unlock Result for operations
        */
        Result* unlock();
};
