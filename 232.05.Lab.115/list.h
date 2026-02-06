/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    David Schaad, Tori Tremelling
 *    Time: 1 hr 15 minutes
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

class TestList; // forward declaration for unit tests
class TestHash; // forward declaration for hash used later

namespace custom
{

    /**************************************************
     * LIST
     * Just like std::list
     **************************************************/
    template <typename T, typename A = std::allocator<T>>
    class list
    {
        friend class ::TestList; // give unit tests access to the privates
        friend class ::TestHash;
        friend void swap(list& lhs, list& rhs);
    public:

        //
        // Construct
        //

        list(const A& a = A()) : numElements(0), pHead(nullptr), pTail(nullptr) {}
		list(const list <T, A>& rhs, const A& a = A()) : numElements(0), pHead(nullptr), pTail(nullptr), alloc(a) 
        {
            for (const auto& item : rhs)
				push_back(item);
        }
        list(list <T, A>&& rhs, const A& a = A());
        list(size_t num, const T& t, const A& a = A());
        list(size_t num, const A& a = A());
		list(const std::initializer_list<T>& il, const A& a = A()) : numElements(0), pHead(nullptr), pTail(nullptr), alloc(a)
        {
			for (const auto& item : il)
                push_back(item);
        }
        template <class Iterator>
		list(Iterator first, Iterator last, const A& a = A()) : numElements(0), pHead(nullptr), pTail(nullptr), alloc(a)
        {
            for (auto it = first; it != last; ++it)
				push_back(*it);
        }
        ~list()
        {
            clear();
        }

        //
        // Assign
        //

        list <T, A>& operator = (list <T, A>& rhs);
        list <T, A>& operator = (list <T, A>&& rhs);
        list <T, A>& operator = (const std::initializer_list<T>& il);
        void swap(list <T, A>& rhs) 
        {
            // Swap pHead
            Node* pTempHead = rhs.pHead;
            rhs.pHead = this->pHead;
            this->pHead = pTempHead;

            // Swap pTail
            Node* pTempTail = rhs.pTail;
            rhs.pTail = this->pTail;
            this->pTail = pTempTail;

            // Swap numElements
            size_t tempElements = rhs.numElements;
            rhs.numElements = this->numElements;
            this->numElements = tempElements;

            // Swap the allocator
            A tempAlloc = rhs.alloc;
            rhs.alloc = this->alloc;
            this->alloc = tempAlloc;
        }

        //
        // Iterator
        //

        class iterator;
        iterator begin() { return iterator(pHead); }
		iterator begin() const { return iterator(pHead); }

        iterator rbegin() { return iterator(pTail); }

        iterator end() { return iterator(nullptr); }
		iterator end() const { return iterator(nullptr); }

        //
        // Access
        //

        T& front();
        T& back();

        //
        // Insert
        //

        void push_front(const T& data);
        void push_front(T&& data);
        void push_back(const T& data);
        void push_back(T&& data);
        iterator insert(iterator it, const T& data);
        iterator insert(iterator it, T&& data);

        //
        // Remove
        //

        void pop_back();
        void pop_front();
        void clear();
        iterator erase(const iterator& it);

        //
        // Status
        //

        bool empty()  const { return !pHead; }
        size_t size() const { return numElements; }

    private:
        // nested linked list class
        class Node;

        // member variables
        A    alloc;         // use alloacator for memory allocation
        size_t numElements; // though we could count, it is faster to keep a variable
        Node* pHead;       // pointer to the beginning of the list
        Node* pTail;       // pointer to the ending of the list
    };

    /*************************************************
     * NODE
     * the node class.  Since we do not validate any
     * of the setters, there is no point in making them
     * private.  This is the case because only the
     * List class can make validation decisions
     *************************************************/
    template <typename T, typename A>
    class list <T, A> ::Node
    {
    public:
        //
        // Construct
        //
        Node() : pNext(nullptr), pPrev(nullptr) {}

        Node(const T& data) : data(data), pNext(nullptr), pPrev(nullptr) {}

        Node(T&& data) : data(std::move(data)), pNext(nullptr), pPrev(nullptr) {}

        //
        // Member Variables
        //

        T data;             // user data
        Node* pNext;       // pointer to next node
        Node* pPrev;       // pointer to previous node
    };

    /*************************************************
     * LIST ITERATOR
     * Iterate through a List, non-constant version
     ************************************************/
    template <typename T, typename A>
    class list <T, A> ::iterator
    {
        friend class ::TestList; // give unit tests access to the privates
        friend class ::TestHash;
        template <typename TT, typename AA>
        friend class custom::list;

    public:
        // constructors, destructors, and assignment operator
        iterator() : p(nullptr) {}
        iterator(Node* pRHS) : p(pRHS) {}
        iterator(const iterator& rhs) : p(rhs.p) {}

        iterator& operator = (const iterator& rhs)
        {
            p = rhs.p;
            return *this;
        }

        // equals, not equals operator
        bool operator == (const iterator& rhs) const { return p == rhs.p; }
        bool operator != (const iterator& rhs) const { return p != rhs.p; }

        // dereference operator, fetch a node
        T& operator * ()
        {
            return p->data;
        }

        // postfix increment
        iterator operator ++ (int postfix)
        {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        // prefix increment
        iterator& operator ++ ()
        {
            //if (p->pNext) // Textbook says to check but unit tests don't want us to???
            p = p->pNext;
            return *this;
        }

        // postfix decrement
        iterator operator -- (int postfix)
        {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        // prefix decrement
        iterator& operator -- ()
        {
            //if (p->pPrev)
            p = p->pPrev;
            return *this;
        }

        // two friends who need to access p directly
        friend iterator list <T, A> ::insert(iterator it, const T& data);
        friend iterator list <T, A> ::insert(iterator it, T&& data);
        friend iterator list <T, A> ::erase(const iterator& it);

    private:

        typename list <T, A> ::Node* p;
    };

    /*****************************************
     * LIST :: NON-DEFAULT constructors
     * Create a list initialized to a value
     ****************************************/
    template <typename T, typename A>
	list <T, A> ::list(size_t num, const T& t, const A& a) : numElements(0), pHead(nullptr), pTail(nullptr), alloc(a)
    {
        for (size_t i = 0; i < num; i++)
            push_back(t);
    }

    /*****************************************
     * LIST :: NON-DEFAULT constructors
     * Create a list initialized to a value
     ****************************************/
    template <typename T, typename A>
    list <T, A> ::list(size_t num, const A& a) : numElements(0), pHead(nullptr), pTail(nullptr)
    {
        for (size_t i = 0; i < num; ++i)
        {
			Node* pNew = new Node();

			pNew->pPrev = pTail;
			pNew->pNext = nullptr;

            if (pTail)
                pTail->pNext = pNew;
            else
				pHead = pNew;

			pTail = pNew;
			++numElements;
        }
    }

    /*****************************************
     * LIST :: MOVE constructors
     * Steal the values from the RHS
     ****************************************/
    template <typename T, typename A>
    list <T, A> ::list(list <T, A>&& rhs, const A& a) :
        numElements(rhs.numElements), pHead(rhs.pHead), pTail(rhs.pTail), alloc(a)
    {
        rhs.pHead = rhs.pTail = nullptr;
        rhs.numElements = 0;
    }

    /**********************************************
     * LIST :: assignment operator - MOVE
     * Copy one list onto another
     *     INPUT  : a list to be moved
     *     OUTPUT :
     *     COST   : O(n) with respect to the size of the LHS
     *********************************************/
    template <typename T, typename A>
    list <T, A>& list <T, A> :: operator = (list <T, A>&& rhs)
    {
        clear();

		swap(rhs);

        return *this;
    }

    /**********************************************
     * LIST :: assignment operator
     * Copy one list onto another
     *     INPUT  : a list to be copied
     *     OUTPUT :
     *     COST   : O(n) with respect to the number of nodes
     *********************************************/
    template <typename T, typename A>
    list <T, A>& list <T, A> :: operator = (list <T, A>& rhs)
    {
        auto itRHS = rhs.begin();
        auto itLHS = begin();

        // While both lists still have existing nodes, replace the data of those nodes
        while ((itRHS != rhs.end()) && (itLHS != end()))
        {
            *itLHS = *itRHS;
            ++itRHS;
            ++itLHS;
        }

        // If there are still nodes in the rhs, add them to the lhs
        while (itRHS != rhs.end())
        {
            push_back(*itRHS);
            ++itRHS;
		}

        while (itLHS != end())
        {
            itLHS = erase(itLHS);
		}

        return *this;
    }

    /**********************************************
     * LIST :: assignment operator
     * Copy one list onto another
     *     INPUT  : a list to be copied
     *     OUTPUT :
     *     COST   : O(n) with respect to the number of nodes
     *********************************************/
    template <typename T, typename A>
    list<T, A>& list<T, A>::operator=(const std::initializer_list<T>& rhs)
    {
        auto itLHS = begin();
        auto itRHS = rhs.begin();

        // Assign while both exist
        while (itLHS != end() && itRHS != rhs.end())
        {
            *itLHS = *itRHS;
            ++itLHS;
            ++itRHS;
        }

        // RHS longer add nodes
        while (itRHS != rhs.end())
        {
            push_back(*itRHS);
            ++itRHS;
        }

        // LHS longer remove nodes
        while (itLHS != end())
            itLHS = erase(itLHS);

        return *this;
    }


    /**********************************************
     * LIST :: CLEAR
     * Remove all the items currently in the linked list
     *     INPUT  :
     *     OUTPUT :
     *     COST   : O(n) with respect to the number of nodes
     *********************************************/
    template <typename T, typename A>
    void list <T, A> ::clear()
    {
        Node* p = pHead;
        while (p != nullptr)
        {
            Node* pNext = p->pNext;
            delete p;
            p = pNext;
        }
        pHead = pTail = nullptr;
        numElements = 0;
    }

    /*********************************************
     * LIST :: PUSH BACK
     * add an item to the end of the list
     *    INPUT  : data to be added to the list
     *    OUTPUT :
     *    COST   : O(1)
     *********************************************/
    template <typename T, typename A>
    void list <T, A> ::push_back(const T& data)
    {
        // Pass 'data' to the constructor to copy it
        Node* pNew = new Node(data);

        pNew->pPrev = pTail;
        pNew->pNext = nullptr;

        if (pTail != nullptr)
            pTail->pNext = pNew;
        else
            pHead = pNew;

        pTail = pNew;
        numElements++;
    }

    template <typename T, typename A>
    void list <T, A> ::push_back(T&& data)
    {
        // Use std::move to pass 'data' to the constructor
        Node* pNew = new Node(std::move(data));

        pNew->pPrev = pTail;
        pNew->pNext = nullptr;

        if (pTail != nullptr)
            pTail->pNext = pNew;
        else
            pHead = pNew;

        pTail = pNew;
        numElements++;
    }

    /*********************************************
     * LIST :: PUSH FRONT
     * add an item to the head of the list
     *     INPUT  : data to be added to the list
     *     OUTPUT :
     *     COST   : O(1)
     *********************************************/
    template <typename T, typename A>
    void list <T, A> ::push_front(const T& data)
    {
        // Pass 'data' to the constructor to copy it
        Node* pNew = new Node(data);

        pNew->pNext = pHead;
        pNew->pPrev = nullptr;

        if (pHead != nullptr)
            pHead->pPrev = pNew;
        else
            pTail = pNew;

        pHead = pNew;
        numElements++;
    }

    template <typename T, typename A>
    void list <T, A> ::push_front(T&& data)
    {
        // Use std::move to pass 'data' to the constructor
        Node* pNew = new Node(std::move(data));

        pNew->pNext = pHead;
        pNew->pPrev = nullptr;

        if (pHead != nullptr)
            pHead->pPrev = pNew;
        else
            pTail = pNew;

        pHead = pNew;
        numElements++;
    }


    /*********************************************
     * LIST :: POP BACK
     * remove an item from the end of the list
     *    INPUT  :
     *    OUTPUT :
     *    COST   : O(1)
     *********************************************/
    template <typename T, typename A>
    void list <T, A> ::pop_back()
    {
        if (!pTail)
            return;

        Node* pRemove = pTail;
        pTail = pTail->pPrev;

        // If there is a node before pRemove, set their pNext to nullptr
        if (pRemove->pPrev)
            pRemove->pPrev->pNext = nullptr;

        // If there isn't a node before pRemove, it means we're at the last element of the list
        else
            pHead = nullptr;

        delete pRemove;
        numElements--;
    }

    /*********************************************
     * LIST :: POP FRONT
     * remove an item from the front of the list
     *    INPUT  :
     *    OUTPUT :
     *    COST   : O(1)
     *********************************************/
    template <typename T, typename A>
    void list <T, A> ::pop_front()
    {
        if (!pHead)
            return;

        Node* pRemove = pHead;
        pHead = pHead->pNext;

        // If there is a node after pRemove, set their pPrev to nullptr
        if (pRemove->pNext)
            pRemove->pNext->pPrev = nullptr;

        // If there isn't a node after pRemove, it means we're at the last element of the list
        else
            pTail = nullptr;

        delete pRemove;
        numElements--;
    }

    /*********************************************
     * LIST :: FRONT
     * retrieves the first element in the list
     *     INPUT  :
     *     OUTPUT : data to be displayed
     *     COST   : O(1)
     *********************************************/
    template <typename T, typename A>
    T& list <T, A> ::front()
    {
        if (!empty())
            return (pHead->data);
        throw("ERROR: unable to access data from an empty list");
    }

    /*********************************************
     * LIST :: BACK
     * retrieves the last element in the list
     *     INPUT  :
     *     OUTPUT : data to be displayed
     *     COST   : O(1)
     *********************************************/
    template <typename T, typename A>
    T& list <T, A> ::back()
    {
        if (!empty())
            return (pTail->data);
        throw("ERROR: unable to access data from an empty list");
    }


    /******************************************
     * LIST :: REMOVE
     * remove an item from the middle of the list
     *     INPUT  : an iterator to the item being removed
     *     OUTPUT : iterator to the new location
     *     COST   : O(1)
     ******************************************/
    template <typename T, typename A>
    typename list <T, A> ::iterator  list <T, A> ::erase(const list <T, A> ::iterator& it)
    {
        if (it == end())
         return end();

        Node* pDelete = it.p;

        Node* pNext = pDelete->pNext;
        Node* pPrev = pDelete->pPrev;

        if (pNext != nullptr)
            pNext->pPrev = pPrev;
        else
            pTail = pPrev;

        if (pPrev != nullptr)
            pPrev->pNext = pNext;
        else
         pHead = pNext;

        delete pDelete;
        numElements--;
     
        return iterator(pNext);
    }

    /******************************************
     * LIST :: INSERT
     * add an item to the middle of the list
     *     INPUT  : data to be added to the list
     *              an iterator to the location where it is to be inserted
     *     OUTPUT : iterator to the new item
     *     COST   : O(1)
     ******************************************/
    template <typename T, typename A>
    typename list <T, A> ::iterator list <T, A> ::insert(list <T, A> ::iterator it,
        const T& data)
    {
        // If the list is empty 
        if (empty() || it == begin())
        {
            push_front(data);
            return begin();
        }

        // If the iterator was pointing to the end
        if (it == end())
        {
            push_back(std::move(data));
            return iterator(pTail);
        }

        // Generl case
        Node* pNext = it.p;
        Node* pPrev = pNext->pPrev;

        // Creating the new Node
        Node* pNew = new Node(data);

        pNew->pNext = pNext;
        pNew->pPrev = pPrev;

        pNext->pPrev = pNew;
        pPrev->pNext = pNew;

        numElements++;
        return iterator(pNew);
    }


    /******************************************
     * LIST :: INSERT
     * add several items into the middle of the list
     *     INPUT  : data to be added to the list
     *              an iterator to the location where it is to be inserted
     *     OUTPUT : iterator to the new item
     *     COST   : O(1)
     ******************************************/
    template <typename T, typename A>
    typename list <T, A> ::iterator list <T, A> ::insert(list <T, A> ::iterator it,
        T&& data)
    {
        // If the list is empty 
        if (empty() || it == begin())
        {
            push_front(std::move(data));
            return begin();
        }

        // If the iterator was pointing to the end
        if (it == end())
        {
            push_back(std::move(data));
            return iterator(pTail);
        }

        // Generl case
        Node* pNext = it.p;
        Node* pPrev = pNext->pPrev;

        // Creating the new Node
        Node* pNew = new Node(std::move(data));

        pNew->pNext = pNext;
        pNew->pPrev = pPrev;

        pNext->pPrev = pNew;
        pPrev->pNext = pNew;

        numElements++;
        return iterator(pNew);
    }

    /**********************************************
     * LIST :: assignment operator - MOVE
     * Copy one list onto another
     *     INPUT  : a list to be moved
     *     OUTPUT :
     *     COST   : O(n) with respect to the size of the LHS
     *********************************************/
    template <typename T, typename A>
    void swap(list <T, A>& lhs, list <T, A>& rhs)
    {
		lhs.swap(rhs);
    }

}; // namespace custom
