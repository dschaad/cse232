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
        deque(const A& a = A()) : alloc(a), numCells(16), numBlocks(0), numElements(0), iaFront(0) // Default cells per block is 16
        {
            data = nullptr;
        }
        deque(const deque& rhs);
        ~deque()
        {
            clear();

            for (size_t ib = 0; ib < numBlocks; ib++)
            {
                if (data && data[ib])
                    std::allocator_traits<A>::deallocate(alloc, data[ib], numCells);
            }

            delete[] data;
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
            assert(numElements != 0);
            return data[ibFromID(0)][icFromID(0)];
        }
        const T& front() const
        {
            assert(numElements != 0);
            return data[ibFromID(0)][icFromID(0)];
        }
        T& back()
        {
            assert(numElements != 0);
            return data[ibFromID(numElements - 1)][icFromID(numElements - 1)];
        }
        const T& back() const
        {
            assert(numElements != 0);
            return data[ibFromID(numElements - 1)][icFromID(numElements - 1)];
        }
        T& operator[](int id)
        {
            assert(0 <= id && id < numElements);
            assert(data[ibFromID(id)] != NULL);
            return data[ibFromID(id)][icFromID(id)];
        }
        const T& operator[](int id) const
        {
            assert(0 <= id && id < numElements);
            assert(data[ibFromID(id)] != NULL);
            return data[ibFromID(id)][icFromID(id)];
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
            int ib = iaFromID(id) / numCells;
            assert(ib < numBlocks);
            return ib;
        }

        // cell index from deque index
        int icFromID(int id) const
        {
            int ic = iaFromID(id) % numCells;
            assert(ic < numCells);
            return ic;
        }

        // reallocate
        void reallocate(int numBlocksNew);

        bool isAllBlocksFilled() const;

        A    alloc;                // use alloacator for memory allocation
        size_t numCells;           // number of cells in a block
        size_t numBlocks;          // number of blocks in the data array
        size_t numElements;        // number of elements in the deque
        int iaFront;               // array-centered index of the front of the deque
        T** data;                  // array of arrays
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
        iterator() : id(0), d(nullptr) {}
        iterator(int id, deque* d) : id(id), d(d)
        {
            this->id = id;
            this->d = d;
        }
        iterator(const iterator& rhs) : id(rhs.id), d(rhs.d) {}

        //
        // Assign
        //
        iterator& operator = (const iterator& rhs)
        {
            id = rhs.id;
            d = rhs.d;
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
            return (*d)[id];
            //return *(new T);
        }

        // 
        // Arithmetic
        //
        int operator - (iterator it) const
        {
            return id - it.id;
        }
        iterator& operator += (int offset)
        {
            id += offset;
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
    deque <T, A> ::deque(const deque& rhs)
      : alloc(rhs.alloc),
        numCells(16),                 // normalize to default cells-per-block expected by tests
        numBlocks(0),
        numElements(rhs.numElements),
        iaFront(0),
        data(nullptr)
    {
       // empty source -> empty destination
       if (numElements == 0 || rhs.data == nullptr || rhs.numBlocks == 0)
          return;

       // create a single block big enough to hold all elements contiguously
       numBlocks = 1;
       data = new T*[numBlocks];
       for (size_t ib = 0; ib < numBlocks; ++ib)
          data[ib] = nullptr;

       data[0] = std::allocator_traits<A>::allocate(alloc, numCells);

       // construct each element sequentially in the single destination block
       for (size_t id = 0; id < numElements; ++id)
       {
          int srcIb = rhs.ibFromID(static_cast<int>(id));
          int srcIc = rhs.icFromID(static_cast<int>(id));

          std::allocator_traits<A>::construct(alloc, &data[0][static_cast<int>(id)], rhs.data[srcIb][srcIc]);
       }
    }

    /*****************************************
     * DEQUE :: COPY-ASSIGN
     * Allocate the space for the elements and
     * call the copy constructor on each element
     ****************************************/
    template <typename T, typename A>
    deque <T, A>& deque <T, A> :: operator = (deque& rhs)
    {
        // self-assignment
        if (this == &rhs)
            return *this;

        // Same size
        if (numElements == rhs.numElements)
        {
            for (size_t i = 0; i < numElements; i++)
                (*this)[i] = rhs[i];

            return *this;
        }

        // Different size
        clear();

        if (rhs.numElements == 0)
            return *this;

        numCells = 16;
        numBlocks = 1;
        numElements = rhs.numElements;
        iaFront = 0;

        data = new T * [numBlocks];
        data[0] = std::allocator_traits<A>::allocate(alloc, numCells);

        for (size_t i = 0; i < numElements; i++)
        {
            int ib = rhs.ibFromID((int)i);
            int ic = rhs.icFromID((int)i);

            std::allocator_traits<A>::construct(alloc, &data[0][i], rhs.data[ib][ic]);
        }

        return *this;
    }

    /*****************************************
     * DEQUE :: PUSH_BACK
     * add an element to the back of the deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::push_back(const T& t)
    {
        if (numBlocks == 0)
            reallocate(1);

        int icTail;

        if (numElements == 0)
            icTail = numCells - 1;
        else
            icTail = icFromID(numElements - 1);

        if (isAllBlocksFilled() && (icTail == (numCells - 1)))
        {
            if (numBlocks == 0)
                reallocate(1);
            else
                reallocate(numBlocks * 2);
        }

        int ib = ibFromID(numElements);
        if (data[ib] == nullptr)
            data[ib] = std::allocator_traits<A>::allocate(alloc, numCells);


        std::allocator_traits<A>::construct(alloc, &data[ib][icFromID(numElements)], t);
        numElements++;
    }

    /*****************************************
     * DEQUE :: PUSH_BACK - move
     * add an element to the back of the deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::push_back(T&& t)
    {
        if (numBlocks == 0)
            reallocate(1);


        int icTail;

        if (numElements == 0)
            icTail = numCells - 1;
        else
            icTail = icFromID(numElements - 1);

        if (isAllBlocksFilled() && (icTail == (numCells - 1)))
        {
            if (numBlocks == 0)
                reallocate(1);
            else
                reallocate(numBlocks * 2);
        }

        int ib = ibFromID(numElements);
        if (data[ib] == nullptr)
            data[ib] = std::allocator_traits<A>::allocate(alloc, numCells);


        std::allocator_traits<A>::construct(alloc, &data[ib][icFromID(numElements)], std::move(t));
        numElements++;
    }

    /*****************************************
     * DEQUE :: PUSH_FRONT
     * add an element to the front of the deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::push_front(const T& t)
    {
        if (numBlocks == 0)
            reallocate(1);

        int icHead;

        if (numElements == 0)
            icHead = 0;
        else
            icHead = icFromID(0);

        if (isAllBlocksFilled() && (icHead == 0))
        {
            if (numBlocks == 0)
                reallocate(1);
            else
                reallocate(numBlocks * 2);
        }

        if (iaFront != 0)
            iaFront--;
        else
            iaFront = (numBlocks * numCells) - 1;

        int ib = ibFromID(0);
        if (data[ib] == nullptr)
            data[ib] = std::allocator_traits<A>::allocate(alloc, numCells);


        std::allocator_traits<A>::construct(alloc, &data[ib][icFromID(0)], t);
        numElements++;
    }

    /*****************************************
     * DEQUE :: PUSH_FRONT - move
     * add an element to the front of the deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::push_front(T&& t)
    {
        if (numBlocks == 0)
            reallocate(1);

        int icHead;

        if (numElements == 0)
            icHead = 0;
        else
            icHead = icFromID(0);

        if (isAllBlocksFilled() && (icHead == 0))
        {
            if (numBlocks == 0)
                reallocate(1);
            else
                reallocate(numBlocks * 2);
        }

        if (iaFront != 0)
            iaFront--;
        else
            iaFront = (numBlocks * numCells) - 1;

        int ib = ibFromID(0);
        if (data[ib] == nullptr)
            data[ib] = std::allocator_traits<A>::allocate(alloc, numCells);


        std::allocator_traits<A>::construct(alloc, &data[ib][icFromID(0)], std::move(t));
        numElements++;
    }

    /*****************************************
     * DEQUE :: CLEAR
     * Remove all the elements from a deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::clear()
    {
        if (data == nullptr || numBlocks == 0)
        {
            numElements = 0;
            iaFront = 0;
            return;
        }

        // Destroy constructed elements
        for (size_t i = 0; i < numElements; i++)
        {
           int ib = ibFromID(i);
           int ic = icFromID(i);
           std::allocator_traits<A>::destroy(alloc, &data[ib][ic]);
        }

        // Deallocate any blocks that were allocated
        for(size_t ib = 0; ib < numBlocks; ++ib)
        { 
            if (data[ib])
            {
                std::allocator_traits<A>::deallocate(alloc, data[ib], numCells);
                data[ib] = nullptr;
            }
        }
        
        numElements = 0;
        iaFront = 0;
    }

    /*****************************************
     * DEQUE :: POP FRONT
     * Remove the front element from a deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::pop_front()
    {
        assert(numElements > 0);

        int ib = ibFromID(0);
        int ic = icFromID(0);

        std::allocator_traits<A>::destroy(alloc, &data[ib][ic]);
        
        numElements--;

        // if deque is now empty, reset iaFront and deallocate the now-empty block
        if (numElements == 0)
        {
            iaFront = 0;
            if (data && data[ib])
            {
                std::allocator_traits<A>::deallocate(alloc, data[ib], numCells);
                data[ib] = nullptr;
            }
            return;
        }

        // advance the front offset
        iaFront = (iaFront + 1) % (numCells * numBlocks);

        // check whether the block that lost the element now contains any remaining elements
        bool hasAny = false;
        for (size_t id = 0; id < numElements; ++id)
        {
            if (ibFromID(static_cast<int>(id)) == ib)
            {
                hasAny = true;
                break;
            }
        }

        if (!hasAny && data && data[ib])
        {
            std::allocator_traits<A>::deallocate(alloc, data[ib], numCells);
            data[ib] = nullptr;
        }
    }

    /*****************************************
     * DEQUE :: POP_BACK
     * Remove the back element from a deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::pop_back()
    {
        assert(numElements > 0);

        int id = numElements - 1;
        int ib = ibFromID(id);
        int ic = icFromID(id);

        std::allocator_traits<A>::destroy(alloc, &data[ib][ic]);

        // decrement element count
        numElements--;

        // if deque is now empty, reset iaFront
        if (numElements == 0)
        {
            iaFront = 0;
            if (data && data[ib])
            {
                std::allocator_traits<A>::deallocate(alloc, data[ib], numCells);
                data[ib] = nullptr;
            }
            return;
        }
        // check whether the block that lost the element now contains any remaining elements
        bool hasAny = false;
        for (size_t i = 0; i < numElements; ++i)
        {
            if (ibFromID(static_cast<int>(i)) == ib)
            {
                hasAny = true;
                break;
            }
        }

        if (!hasAny && data && data[ib])
        {
            std::allocator_traits<A>::deallocate(alloc, data[ib], numCells);
            data[ib] = nullptr;
        }
    }

    /*****************************************
     * DEQUE :: REALLOCATE
     * Remove all the elements from a deque
     ****************************************/
    template <typename T, typename A>
    void deque <T, A> ::reallocate(int numBlocksNew)
    {
        // Allocate a new array of arrays that is numBlocksNew size
        T** newData = new T * [numBlocksNew];

        for (int i = 0; i < numBlocksNew; i++)
            newData[i] = nullptr;

        // Copy elements in order
        for (size_t id = 0; id < numElements; ++id) 
        {
            int oldIb = ibFromID((int)id); 
            int newIb = (int)id / (int)numCells; 
            if (newData[newIb] == nullptr) 
            { 
                // Transfer ownership of the whole block pointer 
                newData[newIb] = data[oldIb]; 
                data[oldIb] = nullptr;
            } 
        } 
        
        // Deallocate any old blocks not moved 
        for (size_t ib = 0; ib < numBlocks; ++ib) 
            if (data && data[ib]) 
                std::allocator_traits<A>::deallocate(alloc, data[ib], numCells); 
        
        delete [] data; 
        
        data = newData; 
        numBlocks = numBlocksNew; 
        
        // keep iaFront's offset within a block so element positions remain correct 
        iaFront = iaFront % (int)numCells; 
    }

    /*****************************************
     * DEQUE :: IS ALL BLOCKS FILLED?
     * return TRUE if all the blocks are filled
     ****************************************/
    template <typename T, typename A>
    bool deque <T, A> ::isAllBlocksFilled() const
    {
        // We have no choice but to check each block looking for a NULLPTR
        for (size_t ib = 0; ib < numBlocks; ib++)
            if (nullptr == data[ib])
                return false;
        return true;
    }


} // namespace custom
