/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>
#include "vector.h" // for default underlying container

class TestPQueue;    // forward declaration for unit test class

namespace custom
{

    /*************************************************
     * P QUEUE
     * Create a priority queue.
     *************************************************/
    template<class T, class Container = custom::vector<T>, class Compare = std::less<T>>
    class priority_queue
    {
        friend class ::TestPQueue; // give the unit test class access to the privates
        template <class TT, class CContainer, class CCompare>
        friend void swap(priority_queue<TT, CContainer, CCompare>& lhs, priority_queue<TT, CContainer, CCompare>& rhs);

    public:

        //
        // construct
        //
        priority_queue(const Compare& c = Compare()) {}
        priority_queue(const priority_queue& rhs, const Compare& c = Compare()) : container(rhs.container), compare(c) {}
        priority_queue(priority_queue&& rhs, const Compare& c = Compare()) : container(std::move(rhs.container)), compare(c) {}
        template <class Iterator>
        priority_queue(Iterator first, Iterator last, const Compare& c = Compare())
        {
        }
        explicit priority_queue(const Compare& c, Container&& rhs)
        {
        }
        explicit priority_queue(const Compare& c, Container& rhs) {}
        ~priority_queue()
        {
        }

        //
        // Access
        //
        const T& top() const;

        //
        // Insert
        //
        void  push(const T& t);
        void  push(T&& t);

        //
        // Remove
        //
        void  pop();

        //
        // Status
        //
        size_t size()  const
        {
            return container.size();
        }
        bool empty() const
        {
            return container.empty();
        }

    private:

        void heapify();                            // convert the container in to a heap
        bool percolateDown(size_t indexHeap);      // fix heap from index down. This is a heap index!

        Container container;       // underlying container (probably a vector)
        Compare   compare;         // comparision operator

    };

    /************************************************
     * P QUEUE :: TOP
     * Get the maximum item from the heap: the top item.
     ***********************************************/
    template <class T, class Container, class Compare>
    const T& priority_queue <T, Container, Compare> ::top() const
    {
        if (!container.empty())
            return container.front();
        else
            throw std::out_of_range("std:out_of_range"); // Feel like this looks weird, but it's what the unit tests expect
    }

    /**********************************************
     * P QUEUE :: POP
     * Delete the top item from the heap.
     **********************************************/
    template <class T, class Container, class Compare>
    void priority_queue <T, Container, Compare> ::pop()
    {
        container.pop_back();
    }

    /*****************************************
     * P QUEUE :: PUSH
     * Add a new element to the heap, reallocating as necessary
     ****************************************/
    template <class T, class Container, class Compare>
    void priority_queue <T, Container, Compare> ::push(const T& t)
    {
        container.push_back(t);

        size_t index = container.size() - 1;

        while (index > 0 && container[(index - 1) / 2] < container[index])
        {
            size_t parent = (index - 1) / 2;

            using std::swap;
            swap(container[parent], container[index]);

            index = parent;
        }
    }
    template <class T, class Container, class Compare>
    void priority_queue <T, Container, Compare> ::push(T&& t)
    {
        container.push_back(std::move(t));

        size_t index = container.size() - 1;

        while (index > 0 && container[(index - 1) / 2] < container[index])
        {
            size_t parent = (index - 1) / 2;

            using std::swap;
            swap(container[parent], container[index]);
            
            index = parent;
        }
    }

    /************************************************
     * P QUEUE :: PERCOLATE DOWN
     * The item at the passed index may be out of heap
     * order. Take care of that little detail!
     * Return TRUE if anything changed.
     ************************************************/
    template <class T, class Container, class Compare>
    bool priority_queue <T, Container, Compare> ::percolateDown(size_t indexHeap)
    {
        using std::swap;

        // Find the left child and the right child of index
        size_t indexLeft = indexHeap * 2;
        size_t indexRight = indexLeft + 1;
        size_t indexBigger;
        Compare comp; // Instructs how we 'rank' our values

        // Determine which child ranks higher
        if (indexRight <= size() && comp(container[indexLeft - 1], container[indexRight - 1]))
            indexBigger = indexRight;
        else
            indexBigger = indexLeft;

        // If the larger child ranks higher than the parent, swap them
        if (indexBigger <= size() && comp(container[indexHeap - 1], container[indexBigger - 1]))
        {
            swap(container[indexHeap - 1], container[indexBigger - 1]);
            percolateDown(indexBigger);
            return true;
        }

        return false;
    }

    /************************************************
     * P QUEUE :: HEAPIFY
     * Turn the container into a heap.
     ************************************************/
    template <class T, class Container, class Compare>
    void priority_queue <T, Container, Compare> ::heapify()
    {
        for (size_t i = size() / 2; i > 0; i--)
            percolateDown(i);
    }

    /************************************************
     * SWAP
     * Swap the contents of two priority queues
     ************************************************/
    template <class T, class Container, class Compare>
    inline void swap(custom::priority_queue <T, Container, Compare>& lhs,
        custom::priority_queue <T, Container, Compare>& rhs)
    {
        lhs.container.swap(rhs.container);
    }

};
