#include "thread_manager.h"
#include <unistd.h> /* usleep */
#include <x86intrin.h>
#include <memory.h>
#include <iomanip>  // для std::hex и std::setw



ThreadManagerTask::ThreadManagerTask
(
    const std::string aId,
    ThreadManager* aManager
)
:
    id( aId ),
    owner( aManager ),
    handler( nullptr ),
    data( nullptr ),
    dataSize( 0 ),
    state( TASK_CREATING )
{
    /* Create thread with body */
    worker = thread
    (
        [ this ]
        ()
        {
            while( !owner -> isTerminating() )
            {
                {
                    unique_lock <mutex> lock( task_mutex );
                    state = TASK_WAITING;
                    notify();
                    task_cv.wait
                    (
                        lock,
                        [ this ]
                        ()
                        {
                            return state == TASK_RUNNING || owner -> isTerminating();
                        }
                    );
                }

                if( !owner -> isTerminating() )
                {
                    if( handler != nullptr )
                    {
                        handler( data );
                        handler = nullptr;
                    }

                    owner -> taskComplete();
                }
            }

            state = TASK_TERMINATED;

        }
    );
}



/*
    Destructor
*/
ThreadManagerTask::~ThreadManagerTask()
{
    join();
    freeData();
}




/*
    Run task
*/
ThreadManagerTask* ThreadManagerTask::run
(
    ThreadManagerHandler aHandler,
    void* aData,
    size_t aDataSize
)
{
    /* Lock task */
    unique_lock <mutex> lock( task_mutex );

    /* Waiting finish of task or terminating */
    task_cv.wait
    (
        lock,
        [ this ]
        ()
        {
            return state == TASK_WAITING || owner -> isTerminating();
        }
    );

    if( !owner -> isTerminating() )
    {
        /* Set handler */
        handler = aHandler;

        /* Free data */
        freeData();

        /* Let new buffer */
        if( aData != nullptr && aDataSize != 0 )
        {
            dataSize = aDataSize;
            data = ::operator new( dataSize );
            memcpy( data, aData, dataSize );
        }

        owner -> taskRun();
        state = TASK_RUNNING;
        notify();
    }

    return this;
}




/******************************************************************************
    ThreadManager
*/



/*
    Constructor
*/
ThreadManager::ThreadManager
(
    LogManager* aLogManager
)
{
    logManager = aLogManager;
}



/*
    Destructor
*/
ThreadManager::~ThreadManager()
{
    terminate();
}



/*
    Check threads count and add new thread
    Threads will not deleted
*/
ThreadManagerTask* ThreadManager::add
(
    const std::string& aId
)
{
    ThreadManagerTask* result = nullptr;

    unique_lock <mutex> lck( mtx );

    if( !terminating && !terminated )
    {
        result = byId( aId );
        if( result == nullptr )
        {
            /* Create new task */
            result = ThreadManagerTask::create( aId, this );
            tasks[ aId ] = result;
        }
    }
    return result;
}



/*
    Method to stop a thread by its index
*/
ThreadManager* ThreadManager::terminate()
{
    {
        unique_lock <mutex> lck( mtx );
        terminating = true;
    }

    notifyTasks();
    for( auto& [id, task] : tasks )
    {
        task -> join();
    }

    {
        unique_lock <mutex> lck( mtx );
        for( auto& [id, task] : tasks )
        {
            task -> destroy();
        }
        tasks.clear();
    }

    terminated = true;
    notifyManager();

    return this;
}





/*
    Dont't use it method from application
    Only for internal use
*/
ThreadManager* ThreadManager::taskRun()
{
    runningCount++;
    return this;
}
