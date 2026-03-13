/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An iterator through Vector
 * Author
 *    Tori Tremelling, David Schaad
 ************************************************************************/

#pragma once

#include <cassert>           // because I am paranoid
#include <new>               // std::bad_alloc
#include <memory>            // for std::allocator
#include <initializer_list>  // for the initializer list, of course!

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

   /*****************************************
    * VECTOR
    * Just like the std :: vector <T> class
    ****************************************/
   template <typename T, typename A = std::allocator<T>>
   class vector
   {
      friend class ::TestVector; // give unit tests access to the privates
      friend class ::TestStack;
      friend class ::TestPQueue;
      friend class ::TestHash;
   public:

      //
      // Construct
      //
      vector(const A& a = A()) : data(nullptr), numElements(0), numCapacity(0), alloc(a) {}
      vector(size_t numElements, const A& a = A());
      vector(size_t numElements, const T& t, const A& a = A());
      vector(const std::initializer_list<T>& l, const A& a = A());
      vector(const vector& rhs);
      vector(vector&& rhs);
      ~vector();

      //
      // Assign
      //
      void swap(vector& rhs)
      {
         std::swap(data, rhs.data);
         std::swap(numCapacity, rhs.numCapacity);
         std::swap(numElements, rhs.numElements);
      }
      vector& operator = (const vector& rhs);
      vector& operator = (vector&& rhs)
      {
         clear();
         shrink_to_fit();
         swap(rhs);
         return *this;
      }

      //
      // Iterator
      //
      class iterator;
      iterator begin()
      {
         return iterator(data);
      }
      iterator end()
      {
         return iterator(data + numElements);
      }

      //
      // Access
      //
      T& operator [] (size_t index);
      const T& operator [] (size_t index) const;
      T& front();
      const T& front() const;
      T& back();
      const T& back() const;

      //
      // Insert
      //
      void push_back(const T& t);
      void push_back(T&& t);
      void reserve(size_t newCapacity);
      void resize(size_t newElements);
      void resize(size_t newElements, const T& t);

      //
      // Remove
      //
      void clear()
      {
         for (size_t i = 0; i < numElements; i++)
            std::allocator_traits<decltype(alloc)>::destroy(alloc, &data[i]);
         numElements = 0;
      }
      void pop_back()
      {
         if (numElements > 0)
            std::allocator_traits<decltype(alloc)>::destroy(alloc, &data[--numElements]);
      }
      void shrink_to_fit();

      //
      // Status
      //
      size_t  size()          const { return numElements; }
      size_t  capacity()      const { return numCapacity; }
      bool empty()            const { return size() == 0; }

   private:

      A    alloc;                // use allocator for memory allocation
      T* data;                 // user data, a dynamically-allocated array
      size_t  numCapacity;       // the capacity of the array
      size_t  numElements;       // the number of items currently used
   };

   /**************************************************
    * VECTOR ITERATOR
    * An iterator through vector.  You only need to
    * support the following:
    *   1. Constructors (default and copy)
    *   2. Not equals operator
    *   3. Increment (prefix and postfix)
    *   4. Dereference
    * This particular iterator is a bi-directional meaning
    * that ++ and -- both work.  Not all iterators are that way.
    *************************************************/
   template <typename T, typename A>
   class vector <T, A> ::iterator
   {
      friend class ::TestVector; // give unit tests access to the privates
      friend class ::TestStack;
      friend class ::TestPQueue;
      friend class ::TestHash;
   public:
      // constructors, destructors, and assignment operator
      iterator() : p(nullptr) {}
      iterator(T* p) : p(p) {}
      iterator(const iterator& rhs) { *this = rhs; }
      iterator(size_t index, vector<T>& v) { p = v.data + index; }
      iterator& operator = (const iterator& rhs)
      {
         this->p = rhs.p;
         return *this;
      }

      // equals, not equals operator
      bool operator != (const iterator& rhs) const { return rhs.p != this->p; }
      bool operator == (const iterator& rhs) const { return rhs.p == this->p; }

      // dereference operator
      T& operator * ()
      {
         if (p)
            return *p;
         else
            throw "ERROR: Trying to dereference a NULL pointer";
      }

      // prefix increment
      iterator& operator ++ ()
      {
         p++;
         return *this;
      }

      // postfix increment
      iterator operator ++ (int postfix)
      {
         iterator temp(*this);
         p++;
         return temp;
      }

      // prefix decrement
      iterator& operator -- ()
      {
         p--;
         return *this;
      }

      // postfix decrement
      iterator operator -- (int postfix)
      {
         iterator temp(*this);
         p--;
         return temp;
      }

   private:
      T* p;
   };


   /*****************************************
    * VECTOR :: NON-DEFAULT constructors
    * non-default constructor: set the number of elements,
    * construct each element, and copy the values over
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(size_t num, const T& t, const A& a) : data(nullptr), numElements(0), numCapacity(0), alloc(a)
   {
      if (num > 0)
      {
         data = std::allocator_traits<decltype(alloc)>::allocate(alloc, num);
         numCapacity = num;

         std::uninitialized_fill_n(data, num, t);
         numElements = num;
      }
   }


   /*****************************************
    * VECTOR :: NON-DEFAULT constructors
    * non-default constructor: set the number of elements,
    * construct each element, and copy the values over
    ****************************************/
    //template <typename T, typename A>
    //vector <T, A> ::vector(size_t num, const T& t, const A& a)
    //{
    //    data = nullptr;
    //    numElements = 19;
    //    numCapacity = 29;
    //}

   /*****************************************
    * VECTOR :: INITIALIZATION LIST constructors
    * Create a vector with an initialization list.
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(const std::initializer_list<T>& l, const A& a) : data(nullptr), numElements(0), numCapacity(0), alloc(a)
   {
      if (l.size())
      {
         data = std::allocator_traits<decltype(alloc)>::allocate(alloc, l.size());
         numCapacity = l.size();

         int i = 0;
         for (auto it = l.begin(); it != l.end(); ++it)
            std::allocator_traits<decltype(alloc)>::construct(alloc, &data[i++], *it);
         assert(i == l.size());
         numElements = l.size();
      }
   }

   /*****************************************
    * VECTOR :: NON-DEFAULT constructors
    * non-default constructor: set the number of elements,
    * construct each element, and copy the values over
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(size_t num, const A& a) : data(nullptr), numElements(0), numCapacity(0), alloc(a)
   {
      if (num > 0)
      {
         data = std::allocator_traits<decltype(alloc)>::allocate(alloc, num);
         numCapacity = num;

         for (size_t i = 0; i < num; i++)
            std::allocator_traits<decltype(alloc)>::construct(alloc, &data[i]);
         numElements = num;
      }
   }

   /*****************************************
    * VECTOR :: COPY CONSTRUCTOR
    * Allocate the space for numElements and
    * call the copy constructor on each element
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(const vector& rhs) : data(nullptr), numElements(0), numCapacity(0), alloc(rhs.alloc)
   {
      (*this) = rhs;
   }

   /*****************************************
    * VECTOR :: MOVE CONSTRUCTOR
    * Steal the values from the RHS and set it to zero.
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(vector&& rhs) : data(nullptr), numElements(0), numCapacity(0), alloc(rhs.alloc)
   {
      *this = std::move(rhs);
   }

   /*****************************************
    * VECTOR :: DESTRUCTOR
    * Call the destructor for each element from 0..numElements
    * and then free the memory
    ****************************************/
   template <typename T, typename A>
   vector <T, A> :: ~vector()
   {
      if (numCapacity != 0)
      {
         assert(nullptr != data);

         for (size_t i = 0; i < numElements; i++)
            std::allocator_traits<decltype(alloc)>::destroy(alloc, &data[i]);

         std::allocator_traits<decltype(alloc)>::deallocate(alloc, data, numCapacity);
      }
   }

   /***************************************
    * VECTOR :: RESIZE
    * This method will adjust the size to newElements.
    * This will either grow or shrink newElements.
    *     INPUT  : newCapacity the size of the new buffer
    *     OUTPUT :
    **************************************/
   template <typename T, typename A>
   void vector <T, A> ::resize(size_t newElements)
   {
      assert(newElements >= 0);

      if (newElements < numElements)
      {
         for (size_t i = newElements; i < numElements; i++)
            std::allocator_traits<decltype(alloc)>::destroy(alloc, &data[i]);
      }

      else if (newElements > numElements)
      {
         if (newElements > numCapacity)
            reserve(newElements);
         for (size_t i = numElements; i < newElements; i++)
            std::allocator_traits<decltype(alloc)>::construct(alloc, &data[i]);
      }

      numElements = newElements;
   }

   template <typename T, typename A>
   void vector <T, A> ::resize(size_t newElements, const T& t)
   {
      assert(newElements >= 0);

      if (newElements < numElements)
      {
         for (size_t i = newElements; i < numElements; i++)
            std::allocator_traits<decltype(alloc)>::destroy(alloc, &data[i]);
      }
      else if (newElements > numElements)
      {
         if (newElements > numCapacity)
            reserve(newElements);
         for (size_t i = numElements; i < newElements; i++)
            std::allocator_traits<decltype(alloc)>::construct(alloc, &data[i], t);
      }

      numElements = newElements;
   }

   /***************************************
    * VECTOR :: RESERVE
    * This method will grow the current buffer
    * to newCapacity.  It will also copy all
    * the data from the old buffer into the new
    *     INPUT  : newCapacity the size of the new buffer
    *     OUTPUT :
    **************************************/
   template <typename T, typename A>
   void vector <T, A> ::reserve(size_t newCapacity)
   {
      if (newCapacity <= numCapacity)
         return;

      assert(newCapacity > 0 && newCapacity > numCapacity);

      T* pNew = std::allocator_traits<decltype(alloc)>::allocate(alloc, newCapacity);

      for (size_t i = 0; i < numElements; i++)
         std::allocator_traits<decltype(alloc)>::construct(alloc, pNew + i, std::move(data[i]));

      if (nullptr != data)
      {
         for (size_t i = 0; i < numElements; i++)
            std::allocator_traits<decltype(alloc)>::destroy(alloc, &data[i]);

         std::allocator_traits<decltype(alloc)>::deallocate(alloc, data, numCapacity);
      }

      data = pNew;
      numCapacity = newCapacity;
   }

   /***************************************
    * VECTOR :: SHRINK TO FIT
    * Get rid of any extra capacity
    *     INPUT  :
    *     OUTPUT :
    **************************************/
   template <typename T, typename A>
   void vector<T, A>::shrink_to_fit()
   {
      if (numCapacity == numElements)
         return;

      T* pNew = nullptr;

      if (numElements)
      {
         pNew = std::allocator_traits<A>::allocate(alloc, numElements);

         for (size_t i = 0; i < numElements; i++)
            std::allocator_traits<A>::construct(alloc, pNew + i, data[i]);
      }

      for (size_t i = 0; i < numElements; i++)
         std::allocator_traits<A>::destroy(alloc, data + i);

      if (data)
         std::allocator_traits<A>::deallocate(alloc, data, numCapacity);

      data = pNew;
      numCapacity = numElements;
   }


   /*****************************************
    * VECTOR :: SUBSCRIPT
    * Read-Write access
    ****************************************/
   template <typename T, typename A>
   T& vector <T, A> :: operator [] (size_t index)
   {
      assert(index >= 0 && index < numElements);
      return data[index];

   }

   /******************************************
    * VECTOR :: SUBSCRIPT
    * Read-Write access
    *****************************************/
   template <typename T, typename A>
   const T& vector <T, A> :: operator [] (size_t index) const
   {
      assert(index >= 0 && index < numElements);
      return data[index];
   }

   /*****************************************
    * VECTOR :: FRONT
    * Read-Write access
    ****************************************/
   template <typename T, typename A>
   T& vector <T, A> ::front()
   {
      assert(numElements > 0);
      return data[0];
   }

   /******************************************
    * VECTOR :: FRONT
    * Read-Write access
    *****************************************/
   template <typename T, typename A>
   const T& vector <T, A> ::front() const
   {
      return data[0];
   }

   /*****************************************
    * VECTOR :: FRONT
    * Read-Write access
    ****************************************/
   template <typename T, typename A>
   T& vector <T, A> ::back()
   {
      return data[numElements - 1];
   }

   /******************************************
    * VECTOR :: FRONT
    * Read-Write access
    *****************************************/
   template <typename T, typename A>
   const T& vector <T, A> ::back() const
   {
      return data[numElements - 1];
   }

   /***************************************
    * VECTOR :: PUSH BACK
    * This method will add the element 't' to the
    * end of the current buffer.  It will also grow
    * the buffer as needed to accomodate the new element
    *     INPUT  : 't' the new element to be added
    *     OUTPUT : *this
    **************************************/
   template <typename T, typename A>
   void vector <T, A> ::push_back(const T& t)
   {
      if (numCapacity == 0)\
         reserve(1);
      else if (numElements == numCapacity)
         reserve(numCapacity * 2);

      std::allocator_traits<decltype(alloc)>::construct(alloc, &data[numElements++], t);

   }

   template <typename T, typename A>
   void vector <T, A> ::push_back(T&& t)
   {
      if (numCapacity == 0)
         reserve(1);
      else if (numElements == numCapacity)
         reserve(numCapacity * 2);

      std::allocator_traits<decltype(alloc)>::construct(alloc, &data[numElements++], std::move(t));
   }

   /***************************************
    * VECTOR :: ASSIGNMENT
    * This operator will copy the contents of the
    * rhs onto *this, growing the buffer as needed
    *     INPUT  : rhs the vector to copy from
    *     OUTPUT : *this
    **************************************/
   template <typename T, typename A>
   vector <T, A>& vector <T, A> :: operator = (const vector& rhs)
   {
      if (rhs.numElements == numElements)
      {
         for (size_t i = 0; i < rhs.numElements; i++)
            data[i] = rhs.data[i];
      }
      else if (rhs.numElements > numElements)
      {
         if (rhs.numElements <= numCapacity)
         {
            for (size_t i = 0; i < numElements; i++)
               data[i] = rhs.data[i];

            for (size_t i = numElements; i < rhs.numElements; i++)
               std::allocator_traits<decltype(alloc)>::construct(alloc, &data[i], rhs.data[i]);
            numElements = rhs.numElements;
         }
         else
         {
            T* pNew = alloc.allocate(rhs.numElements);

            for (size_t i = 0; i < rhs.numElements; i++)
               std::allocator_traits<decltype(alloc)>::construct(alloc, pNew + i, rhs.data[i]);

            clear();

            std::allocator_traits<decltype(alloc)>::deallocate(alloc, data, numCapacity);

            data = pNew;
            numElements = rhs.numElements;
            numCapacity = rhs.numElements;
         }
      }
      else
      {
         for (size_t i = 0; i < rhs.numElements; i++)
            data[i] = rhs.data[i];

         for (size_t i = rhs.numElements; i < numElements; i++)
            std::allocator_traits<decltype(alloc)>::destroy(alloc, &data[i]);

         numElements = rhs.numElements;
      }

      return *this;
   }
   //template <typename T, typename A>
   //vector <T, A>& vector <T, A> :: operator = (vector&& rhs)
   //{

   //    return *this;
   //}




} // namespace custom

