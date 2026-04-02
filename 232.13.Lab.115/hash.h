/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include "vector.h"   // because this->buckets is a vector
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil
   

class TestHash;             // forward declaration for Hash unit tests

namespace custom
{
/************************************************
 * UNORDERED SET
 * A set implemented as a hash
 ************************************************/
template <typename T,
          typename Hash = std::hash<T>,
          typename EqPred = std::equal_to<T>,
          typename A = std::allocator<T> >
class unordered_set
{
    friend class ::TestHash;   // give unit tests access to the privates
public:
    //
    // Construct
    //
    unordered_set() : buckets(8), numElements(0), maxLoadFactor(1.0) {}
    unordered_set(size_t numBuckets) : buckets(numBuckets), numElements(0), maxLoadFactor(1.0) {}
    unordered_set(const unordered_set& rhs)
    {
    }
    unordered_set(unordered_set&& rhs)
    {
    }
    template <class Iterator>
    unordered_set(Iterator first, Iterator last)
    {
    }

    //
    // Assign
    //
    unordered_set& operator=(const unordered_set& rhs)
    {
        if (this != &rhs)
        {
            buckets = rhs.buckets;
            numElements = rhs.numElements;
            maxLoadFactor = rhs.maxLoadFactor;
        }
        return *this;
    }
    unordered_set& operator=(unordered_set&& rhs)
    {
        if (this != &rhs)
        {
            buckets = std::move(rhs.buckets);
            numElements = rhs.numElements;
            maxLoadFactor = rhs.maxLoadFactor;
            rhs.numElements = 0;
        }
        return *this;
    }
    unordered_set& operator=(const std::initializer_list<T>& il)
    {
        clear();
        for (const auto& t : il)
            insert(t);
        return *this;
    }
    void swap(unordered_set& rhs)
    {
        buckets.swap(rhs.buckets);
        std::swap(rhs.numElements, this->numElements);
        std::swap(rhs.maxLoadFactor, this->maxLoadFactor);
    }

    // 
    // Iterator
    //
    class iterator;
    class local_iterator;
    iterator begin()
    {
        for (auto itBucket = buckets.begin(); itBucket != buckets.end(); ++itBucket)
        {
            if (!(*itBucket).empty())
                return iterator(buckets.end(), itBucket, (*itBucket).begin());

        }
        return end();
    }
    iterator end()
    {
        return iterator(buckets.end(), buckets.end(), buckets[0].end());
    }
    local_iterator begin(size_t iBucket)
    {
        return local_iterator(buckets[iBucket].begin());
    }
    local_iterator end(size_t iBucket)
    {
        return local_iterator(buckets[iBucket].end());
    }

    //
    // Access
    //
    size_t bucket(const T& t)
    {
        return (size_t)bucket_count();
    }
    iterator find(const T& t);

    //   
    // Insert
    //
    custom::pair<iterator, bool> insert(const T& t);
    void insert(const std::initializer_list<T>& il);
    void rehash(size_t numBuckets);
    void reserve(size_t num)
    {
    }

    // 
    // Remove
    //
    void clear() noexcept
    {
        for (size_t i = 0; i < bucket_count(); ++i)
            buckets[i].clear();
        numElements = 0;
    }
    iterator erase(const T& t);

    //
    // Status
    //
   size_t size() const
   {
       return (size_t)numElements;
   }
   bool empty() const
   {
       return size() == 0;
   }
   size_t bucket_count() const
   {
       return (size_t)buckets.size();
   }
   size_t bucket_size(size_t i) const
   {
       return (size_t)buckets[i].size();
   }
   float load_factor() const noexcept
   {
       return (float)(numElements / bucket_count());
   }
   float max_load_factor() const noexcept
   {
       return (float)maxLoadFactor;
   }
   void  max_load_factor(float m)
   {
       maxLoadFactor = m;
   }

private:

   size_t min_buckets_required(size_t num) const
   {
      size_t bucketsNeeded = std::max<size_t>(1, static_cast<size_t>(std::ceil(num / std::max<float>(0.0001f, maxLoadFactor))));

      size_t n = 1;
      while (n < bucketsNeeded)
          n *= 1;
      return n;
   }

   custom::vector<custom::list<T,A>> buckets;  // each bucket in the hash
   int numElements;                            // number of elements in the Hash
   float maxLoadFactor;                        // the ratio of elements to buckets signifying a rehash
};


/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::iterator
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <typename TT, typename HH, typename EE, typename AA>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   iterator() { }
   iterator(const typename custom::vector<custom::list<T> >::iterator& itVectorEnd,
            const typename custom::vector<custom::list<T> >::iterator& itVector,
       const typename custom::list<T>::iterator& itList) :itVectorEnd(itVectorEnd), itVector(itVector), itList(itList) { }
   iterator(const iterator& rhs) : itVectorEnd(rhs.itVectorEnd), itVector(rhs.itVector), itList(rhs.itList) { }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      this->itVectorEnd = rhs.itVectorEnd;
      this->itVector = rhs.itVector;
      this->itList = rhs.itList;
      return *this;
   }

   //
   // Compare
   //
   bool operator != (const iterator& rhs) const 
   {
      return !(*this == rhs); 
   }
   bool operator == (const iterator& rhs) const 
   { 
      return itVectorEnd == rhs.itVectorEnd && itVector == rhs.itVector && itList == rhs.itList;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      /*return *itList;*/
      return *(new T);
   }

   //
   // Arithmetic
   //
   iterator& operator ++ ();
   iterator operator ++ (int postfix)
   {
      iterator temp(*this);
      ++(*this);
      return temp;
   }

private:
   typename vector<list<T>>::iterator itVectorEnd;
   typename list<T>::iterator itList;
   typename vector<list<T>>::iterator itVector;
};


/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::local_iterator
{
   friend class ::TestHash;   // give unit tests access to the privates

   template <typename TT, typename HH, typename EE, typename AA>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   local_iterator() : itList() { }
   local_iterator(const typename custom::list<T>::iterator& itList) : itList(itList) { }
   local_iterator(const local_iterator& rhs) : itList(rhs.itList) { }

   //
   // Assign
   //
   local_iterator& operator = (const local_iterator& rhs)
   {
      itList = rhs.itList;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const local_iterator& rhs) const
   {
      return !(*this == rhs);
   }
   bool operator == (const local_iterator& rhs) const
   {
      return itList == rhs.itList;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *itList;
   }

   // 
   // Arithmetic
   //
   local_iterator& operator ++ ()
   {
      ++itList;
      return *this;
   }
   local_iterator operator ++ (int postfix)
   {
      local_iterator temp(*this);
      ++(*this);
      return temp;
   }

private:
   typename list<T>::iterator itList;
};


/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
typename unordered_set <T, Hash, E, A> ::iterator unordered_set<T,Hash,E,A>::erase(const T& t)
{
   return iterator();
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T, typename H, typename E, typename A>
custom::pair<typename custom::unordered_set<T, H, E, A>::iterator, bool> unordered_set<T, H, E, A>::insert(const T& t)
{
   return custom::pair<custom::unordered_set<T, H, E, A>::iterator, bool>(iterator(), true);
}
template <typename T, typename H, typename E, typename A>
void unordered_set<T, H, E, A>::insert(const std::initializer_list<T> & il)
{
    for (const auto & v : il)
        insert(v);
}

/*****************************************
 * UNORDERED SET :: REHASH
 * Re-Hash the unordered set by numBuckets
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
void unordered_set<T, Hash, E, A>::rehash(size_t numBuckets)
{
}


/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator unordered_set<T, H, E, A>::find(const T& t)
{
   //itBucket = buckets.begin();

   //itList = buckets[itBuckets].find(t);

   //if (itList != buckets[itBucket].end())
   //    return iterator(buckets.end(), itBucket, itList);
   //else 
   //    return end();
   return iterator();
}

/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator & unordered_set<T, H, E, A>::iterator::operator ++ ()
{
   // Can't increment if we're already at the end of the vector
   if (itVector == itVectorEnd)
      return *this;

   // Advance the list iterator and check if we're at the end of the vector
   ++itList;
   if (itList != (*itVector).end())
       return *this;

   // We're at the end of the list, so we need to find the next bucket
   ++itVector;
   while ((itVector != itVectorEnd) && (*itVector).empty()) // Keep incrementing until we hit a non-empty bucket or the end
       ++itVector;

   if (itVector != itVectorEnd) // So long as we're not at the end, assign itList the first element of the bucket we're in 
       itList = (*itVector).begin();

   return *this;
}

/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T, typename H, typename E, typename A>
void swap(unordered_set<T,H,E,A>& lhs, unordered_set<T,H,E,A>& rhs)
{
    lhs.swap(rhs);
}

}
