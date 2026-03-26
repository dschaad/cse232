/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    David Schaad, Tori Tremelling
 ************************************************************************/

#pragma once

 // Debug stuff
#include <cassert>
#include <memory>   // for std::allocator

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

    /******************************************************
     * DEQUE
     *****************************************************/
    template <typename T, typename A = std::allocator<T>>
    class deque
    {
        friend class ::TestDeque; // give unit tests access to the privates
    public:

        // 
        // Construct
        //
        deque(const A& a = A()) : alloc(a), numCells(0), numBlocks(0), numElements(0), iaFront(0)
        {
            data = nullptr;
        }
        deque(deque& rhs);
        ~deque()
        {
        }

        //
        // Assign
        //
        deque& operator = (deque& rhs);

        // 
        // Iterator
        //
        class iterator;
        iterator begin()
        {
            return iterator(0, this);
        }
        iterator end()
        {
            return iterator(numElements, this);
        }

        // 
        // Access
        //
        T& front()
        {
            //assert(numElements != 0);
            //int ib = ibFromID(0);
            //int ic = icFromID(0);   
            //return data[ib][ic];
            return *(new T);
        }
        const T& front() const
        {
        //  assert(numElements != 0);
        //  int ib = ibFromID(0);
        //  int ic = icFromID(0);
        //  return data[ib][ic];
            return *(new T);
        }
        T& back()
        {
            //assert(numElements != 0);
            //int ib = ibFromID(numElements - 1);
            //int ic = icFromID(numElements - 1);
            //return data[ib][ic];
            return *(new T);
        }
        const T& back() const
        {
            //assert(numElements != 0);
            //int ib = ibFromID(numElements - 1);
            //int ic = icFromID(numElements - 1);
            //return data[ib][ic];
            return *(new T);
        }
        T& operator[](int id)
        {
            //int ib = ibFromID(id);
            //int ic = icFromID(id);
            //return data[ib][ic];
            return *(new T);
        }
        const T& operator[](int id) const
        {
            //int ib = ibFromID(id);
            //int ic = icFromID(id);
            //return data[ib][ic];
            return *(new T);
        }

        //
        // Insert
        //
        void push_back(const T& t);
        void push_back(T&& t);
        void push_front(const T& t);
        void push_front(T&& t);

        //
        // Remove
        //
        void pop_front();
        void pop_back();
        void clear();

        //
        // Status
        //
        size_t size()  const { return numElements; }
        bool   empty() const 
        { 
            if (size() == 0)
                return true;
            else 
                return false;
        }

    private:
        // array index from deque index
        int iaFromID(int id) const
        {
            if (numCells == 0 || numBlocks == 0)
                return 0;

            return (id + iaFront) % (numCells * numBlocks);
        }

        // block index from deque index
        int ibFromID(int id) const
        {
            //int ia = iaFromID(id);
            //return ia / numCells;
            return -1;
        }

        // cell index from deque index
        int icFromID(int id) const
        {
            //int ia = iaFromID(id);
            //return ia % numCells;
            return -1;
        }

        // reallocate
        void reallocate(int numBlocksNew);

        A    alloc;                // use alloacator for memory allocation
        size_t numCells;           // number of cells in a block
        size_t numBlocks;          // number of blocks in the data array
        size_t numElements;        // number of elements in the deque
        int iaFront;               // array-centered index of the front of the deque
        T** data;                 // array of arrays
    };

    /**************************************************
     * DEQUE ITERATOR
     * An iterator through deque.  You only need to
     * support the following:
     *   1. Constructors (default and copy)
     *   2. Not equals operator
     *   3. Increment (prefix and postfix)
     *   4. Dereference
     * This particular iterator is a bi-directional meaning
     * that ++ and -- both work.  Not all iterators are that way.
     *************************************************/
    template <typename T, typename A>
    class deque <T, A> ::iterator
    {
        friend class ::TestDeque; // give unit tests access to the privates
    public:
        // 
        // Construct
        //
        iterator() : id(0), d(nullptr) { }
        iterator(int id, deque* d) : id(id), d(d)
        {
            this->id = id;
            this->d = d;
        }
        iterator(const iterator& rhs) : id(rhs.id), d(rhs.d) { }

        //
        // Assign
        //
        iterator& operator = (const iterator& rhs)
        {
            return *this;
        }

        // 
        // Compare
        //
        bool operator != (const iterator& rhs) const { return id != rhs.id; }
        bool operator == (const iterator& rhs) const { return id == rhs.id; }

        // 
        // Access
        //
        T& operator * ()
        {
            //return (*d)[id];
            return *(new T);
        }

        // 
        // Arithmetic
        //
        int operator - (iterator it) const
        {
            return 99;
        }
        iterator& operator += (int offset)
        {
            return *this;
        }
        iterator& operator ++ ()
        {
            ++id;
            return *this;
        }
        iterator operator ++ (int postfix)
        {
            iterator temp(*this);
            ++id;
            return temp;
        }
        iterator& operator -- ()
        {
            --id;
            return *this;
        }
        iterator operator -- (int postfix)
        {
            iterator temp(*this);
            --id;
            return temp;
        }

    private:
        int id;
        deque* d;
    };

    /*****************************************
     * DEQUE :: COPY CONSTRUCTOR
     * Allocate the space for the elements and
     * call the copy constructor on each element
     ****************************************/
    template <typename T, typename A>
    deque <T, A> ::deque(deque& rhs)
    {
    }

    /*****************************************
     * DEQUE :: COPY-ASSIGN
     * Allocate the space for the elements and
     * call the copy constructor on each element
     ****************************************/
    template <typename T, typename A>
    deque <T, A>& deque <T, A> :: operator = (deque& rhs)
    {
        return *this;
    }

    /*****************************************
     * DEQUE :: PUSH_BACK
     * add an element to the back of the deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::push_back(const T& t)
    {
        //if (numElements == numCells * numBlocks)
        //    reallocate(numBlocks * 2 + 1);

        //int ib = ibFromID(numElements);
        //int ic = icFromID(numElements);

        //data[ib][ic] = t;
        //numElements++;
    }

    /*****************************************
     * DEQUE :: PUSH_BACK - move
     * add an element to the back of the deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::push_back(T&& t)
    {
    }

    /*****************************************
     * DEQUE :: PUSH_FRONT
     * add an element to the front of the deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::push_front(const T& t)
    {
    }

    /*****************************************
     * DEQUE :: PUSH_FRONT - move
     * add an element to the front of the deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::push_front(T&& t)
    {
    }

    /*****************************************
     * DEQUE :: CLEAR
     * Remove all the elements from a deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::clear()
    {
    }

    /*****************************************
     * DEQUE :: POP FRONT
     * Remove the front element from a deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::pop_front()
    {
    }

    /*****************************************
     * DEQUE :: POP BACK
     * Remove the back element from a deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::pop_back()
    {
    }

    /*****************************************
     * DEQUE :: REALLOCATE
     * Remove all the elements from a deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::reallocate(int numBlocksNew)
    {
        //T** newData = new T * [numBlocksNew];

        //for (int i = 0; i < numBlocksNew; i++)
        //    newData[i] = alloc.allocate(numCells);

        //data = newData;
        //numBlocks = numBlocksNew;
        //iaFront = 0;
    }




} // namespace custom
