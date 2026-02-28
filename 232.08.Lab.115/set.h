/***********************************************************************
* Header:
*    Set
* Summary:
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
*        set                 : A class that represents a Set
*        set::iterator       : An iterator through Set
* Author
*    David Schaad, Tori Tremelling
************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less

class TestSet;        // forward declaration for unit tests

namespace custom
{

    //   class TestSet;

    /************************************************
     * SET
     * A class that represents a Set
     ***********************************************/
    template <typename T>
    class set
    {
        friend class ::TestSet; // give unit tests access to the privates
    public:

        // 
        // Construct
        //
        set() {}
        set(const set& rhs) : bst(rhs.bst) {}
        set(set&& rhs) : bst(std::move(rhs.bst)) {}

        set(const std::initializer_list <T>& il)
        {
            clear();
            insert(il);
        }

        template <class Iterator>
        set(Iterator first, Iterator last)
        {
            clear();
            insert(first, last);
        }
        ~set() {}

        //
        // Assign
        //

        set& operator = (const set& rhs)
        {
            bst = rhs.bst;
            return *this;
        }
        set& operator = (set&& rhs)
        {
            clear();
            swap(rhs);
            return *this;
        }
        set& operator = (const std::initializer_list <T>& il)
        {
            clear();
            insert(il);
            return *this;
        }
        void swap(set& rhs) noexcept
        {
            bst.swap(rhs.bst);
        }

        //
        // Iterator
        //

        class iterator;
        iterator begin() const noexcept
        {
            return bst.begin();
        }
        iterator end() const noexcept
        {
            return bst.end();
        }

        //
        // Access
        //
        iterator find(const T& t)
        {
            return bst.find(t);
        }

        //
        // Status
        //
        bool empty() const noexcept
        {
            return bst.empty();
        }
        size_t size() const noexcept
        {
            return bst.size();
        }

        //
        // Insert
        //
        std::pair<iterator, bool> insert(const T& t)
        {
            std::pair<iterator, bool> bst_pair = bst.insert(t, true);
            return std::pair<iterator, bool>(iterator(bst_pair.first), bst_pair.second);
        }
        std::pair<iterator, bool> insert(T&& t)
        {
            std::pair<iterator, bool> bst_pair = bst.insert(std::move(t), true);
            return std::pair<iterator, bool>(iterator(bst_pair.first), bst_pair.second);
        }
        void insert(const std::initializer_list <T>& il)
        {
            for (const auto& element : il)
                insert(element);
        }
        template <class Iterator>
        void insert(Iterator first, Iterator last)
        {
            for (auto it = first; it != last; ++it)
                insert(*it);
        }


        //
        // Remove
        //
        void clear() noexcept
        {
            bst.clear();
        }
        iterator erase(iterator& it)
        {
            return iterator(bst.erase(it.it));
        }
        size_t erase(const T& t)
        {
            auto it = find(t);

            if (it == end())
                return 0;

            erase(it);
            return 1;
        }
        iterator erase(iterator& itBegin, iterator& itEnd)
        {
            while (itBegin != itEnd)
                itBegin = erase(itBegin);

            return itEnd;
        }

    private:

        custom::BST <T> bst;
    };


    /**************************************************
     * SET ITERATOR
     * An iterator through Set
     *************************************************/
    template <typename T>
    class set <T> ::iterator
    {
        friend class ::TestSet; // give unit tests access to the privates
        friend class custom::set<T>;
    public:
        // constructors, destructors, and assignment operator
        iterator() {}
        iterator(const typename custom::BST<T>::iterator& itRHS) : it(itRHS) {}
        iterator(const iterator& rhs) : it(rhs.it) {}

        iterator& operator = (const iterator& rhs)
        {
            it = rhs.it;
            return *this;
        }

        // equals, not equals operator
        bool operator != (const iterator& rhs) const
        {
            return it != rhs.it;
        }
        bool operator == (const iterator& rhs) const
        {
            return it == rhs.it;
        }

        // dereference operator: by-reference so we can modify the Set
        const T& operator * () const
        {
            return *it;
        }

        // prefix increment
        iterator& operator ++ ()
        {
            ++it;
            return *this;
        }

        // postfix increment
        iterator operator++ (int)
        {
            iterator temp(*this);
            ++it;
            return temp;
        }

        // prefix decrement
        iterator& operator -- ()
        {
            --it;
            return *this;
        }

        // postfix decrement
        iterator operator-- (int)
        {
            iterator temp(*this);
            --it;
            return temp;
        }

    private:

        typename custom::BST<T>::iterator it;
    };



}; // namespace custom
