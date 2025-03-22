#pragma once

#include <cstddef>      /* for NULL */



using namespace std;



class Heap
{
    private:

        void** items = NULL;
        int count = 0;

        Heap* setItems
        (
            void **
        );

    public:


        /*
            Constructor
        */
        Heap();



        /*
            Destructor
        */
        virtual ~Heap();



        /*
            Create and return heap
        */
        static Heap* create();



        /*
            Destroy heap
        */
        virtual void destroy();



        /*
            Return count elements in heap
        */
        int getCount()
        {
            return count;
        }



        /*
            Return Neurons list count elements
        */
        void** getItems()
        {
            return items;
        }




        /*
            Add neurons from argument list to this list
        */
        Heap* add
        (
            Heap*
        );



        /*
            Return index by element
        */
        int indexBy
        (
            void*
        );



        /*
            Return neuron by index
        */
        void* getByIndex
        (
            int
        );



        /*
            Return neuron by index
        */
        Heap* setByIndex
        (
            int,
            void*
        );



        /*
            Resize
        */
        Heap* resize
        (
            int = 0
        );



        /*
            Expand the heap
        */
        Heap* expand
        (
            int /* Size of expanding */
        );



        /*
            Expand the heap
        */
        Heap* push
        (
            void*
        );



        /*
            Remove one element and resize object
        */
        virtual void* remove
        (
            int
        );


        /*
            Remove one element and resize object
        */
        virtual void* remove
        (
            void*
        );



        /*
            Remove with lyambda
        */
        template <typename Func> Heap* remove
        (
            Func callback,
            Heap* aRemove
        )
        {
            auto c = getCount();

            if( aRemove != NULL )
            {
                aRemove -> resize( c );
            }

            int countRemove = 0;
            int countKeep = 0;

            for( int i = 0; i < c; i++ )
            {
                if( callback( items[ i ] ))
                {
                    if( aRemove != NULL )
                    {
                        aRemove -> setByIndex( countRemove, items[ i ] );
                        countRemove++;
                    }
                }
                else
                {
                    items[ countKeep ] = items[ i ];
                    countKeep++;
                }
            }

            if( aRemove != NULL )
            {
                aRemove -> resize( countRemove );
            }

            resize( countKeep );

            return this;
        };




//        /*
//            Remove all elements form  this,
//            contains in the argument
//        */
//        virtual Heap* remove
//        (
//            Heap* aRemove
//        );
//


        /*
            Remove some elememnts from start index
        */
        Heap* bite
        (
            int,    /* Strat index */
            int     /* Count for bite */
        );



        /*
            Loop with lambda function over all items in the heap.
            Parameters:
                callback: A lambda function taking a pointer to an item in the heap and
                    returning a boolean value indicating whether the iteration should continue.
                    Note: The lambda function should have the following prototype:
                        bool callback(void* item)

            Returns:
                A pointer to the heap object, allowing method chaining.

            Example:
                // Assume heap is an instance of Heap
                // This lambda will print each item in the heap
                heap->loop(
                    [](void* item) {
                        cout << "Item: " << item << endl;
                        // Continue iteration
                        return false;
                    }
                );
        */
        template <typename Func> Heap* loop
        (
            /* Lambda function applied to each item */
            Func callback
        )
        {
            /*
                Iterate over all items in the heap
                and apply the lambda function to each item.
                If the lambda function returns true for any item,
                the iteration stops.
            */
            bool stop = false;
            auto c = getCount();
            auto items = getItems();

            for( int i = 0; i < c && !stop; i++ )
            {
                /* Apply the lambda function to the current item */
                stop = callback( items[ i ] );
            }

            /* Return a pointer to the heap object */
            return this;
        };




        /*
            Return first element or null if not exists
        */
        void* getFirst();



        /*
            Merge two heaps
            Each elemento of Argument heap will add to This heap
            if it not exists in This
        */
        virtual Heap* merge
        (
            Heap* /* Heap for merging */
        );



        /*
            Merge two heaps
            Each elemento of Argument heap will add to This heap
            if it not exists in This
        */
        template <typename Func> Heap* merge
        (
            Heap* a,
            Func callback
        )
        {
            /* Get size of arguments */
            int c = a -> getCount();

            /* Create the new heap with size equals the arguemnts */
            auto searched = Heap::create() -> resize( c );

            /*
                New elements searching for this in argiments
                and store it to the Searched.
            */
            int addIndex = 0;
            for( int i = 0; i < c; i++ )
            {
                auto item = a -> getByIndex( i );
                if( callback( item ))
                {
                    searched -> setByIndex( addIndex, item );
                    addIndex++;
                }
            }

            /* Cut the list of searched elements */
            searched -> resize( addIndex );

            /* Add list of new heap in to this */
            add( searched );

            /* Destroy list of new heap */
            searched -> destroy();

            return this;
        }
};
