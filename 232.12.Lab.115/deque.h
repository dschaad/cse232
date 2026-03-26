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
         assert(numElements != 0);
         assert(data[ibFromID(0)] != NULL);
         return data[ibFromID(0)][icFromID(0)];
      }
      const T& front() const
      {
         assert(numElements != 0);
         assert(data[ibFromID(0)] != NULL);
         return data[ibFromID(0)][icFromID(0)];
      }
      T& back()
      {
         assert(numElements != 0);
         assert(data[ibFromID(numElements - 1)] != NULL);
         return data[ibFromID(numElements - 1)][icFromID(numElements - 1)];
      }
      const T& back() const
      {
         assert(numElements != 0);
         assert(data[ibFromID(numElements - 1)] != NULL);
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
         assert(ib <= numBlocks);
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
   deque <T, A> ::deque(deque& rhs) : alloc(rhs.alloc), numCells(0), numBlocks(0), numElements(0), iaFront(0)
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
      //auto itLHS = this->begin();
      //auto itRHS = rhs.begin();

      //while ((itLHS != end()) && (itRHS != rhs.end()))
      //{
      //   *itLHS = *itRHS;
      //   ++itLHS;
      //   ++itRHS;
      //}

      ////erase(itLHS, end());
      //while (itRHS != rhs.end())
      //{
      //   push_back(*itRHS);
      //   ++itRHS;
      //}
      //return *this;
   }

   /*****************************************
    * DEQUE :: PUSH_BACK
    * add an element to the back of the deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::push_back(const T& t)
   {
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


      std::allocator_traits<A>::construct(alloc, &data[ib][ icFromID(numElements) ], t);
      numElements++;
   }

   /*****************************************
    * DEQUE :: PUSH_BACK - move
    * add an element to the back of the deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::push_back(T&& t)
   {
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


      std::allocator_traits<A>::construct(alloc, &data[ib][ icFromID(0) ], t);
      numElements++;
   }

   /*****************************************
    * DEQUE :: PUSH_FRONT - move
    * add an element to the front of the deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::push_front(T&& t)
   {
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
      //for (int id = 0; id < numElements - 1; id++)
      //   std::allocator_traits<A>::destroy(alloc, &data[ ibFromID(id) ][ icFromID(id) ]);
      //   //delete data[ ibFromID(id) ][ icFromID(id) ];

      //for (int ib = 0; ib < numBlocks; ib++)
      //{
      //   if (data[ib] != nullptr)
      //   {
      //      //std::allocator_traits<A>::destroy(alloc, &data[ib]);
      //      delete data[ib];
      //      data[ib] = nullptr;
      //   }
      //}
      //numElements = 0;
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
      // Allocate a new array of arrays that is numBlocksNew size
      T** newData = new T * [numBlocksNew];

      // Starting from 0 of the current deque index (id), loop through all elements in the deque.
      // Copy the blocks over to the new array in order (unwrap)
      int newIb = 0;
      for (int oldId = 0; oldId < numElements; oldId += numCells)
      {
         newData[newIb] = data[ibFromID(oldId)];
         newIb++;
      }

      // If we have excess blocks, set them to nullptr
      while (newIb < numBlocksNew)
      {
         newData[newIb] = nullptr;
         newIb++;
      }

      // Delete our existing data
      if (data)
         delete data;

      // Re-assign our member variables based on the new data
      data = newData;
      numBlocks = numBlocksNew;
      iaFront = (iaFront % numCells);
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
