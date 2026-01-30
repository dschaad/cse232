/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    David Schaad, Tori Tremelling
 * Time:
 *    2hrs 0 mins
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL

 /*************************************************
  * NODE
  * the node class.  Since we do not validate any
  * of the setters, there is no point in making them
  * private.  This is the case because only the
  * List class can make validation decisions
  *************************************************/
template <class T>
class Node
{
public:
    //
    // Construct
    //
    //
    // Construct
    //

    Node() : pNext(nullptr), pPrev(nullptr) {}

    Node(const T& data) : data(data), pNext(nullptr), pPrev(nullptr) {}

    Node(T&& data) : data(std::move(data)), pNext(nullptr), pPrev(nullptr) {}

    //
    // Member variables
    //

    T data;                 // user data
    Node <T>* pNext;       // pointer to next node
    Node <T>* pPrev;       // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node <T>* copy(const Node <T>* pSource)
{
    if (!pSource)
        return nullptr;

    Node <T>* pDestination = new Node<T>(pSource->data);

    // We need a way to add onto the linked list WITHOUT changing the pointer to the head (pDestination)
    Node <T>* pDes = pDestination;

    // Loop through pSource, starting from the second node (if it exists)
    for (auto p = pSource->pNext; p; p = p->pNext)
        pDes = insert(pDes, p->data, true);
    return pDestination;
}

/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node <T>*& pDestination, const Node <T>* pSource)
{
    const Node<T>* pSrc = pSource;
    Node<T>* pDes = pDestination;
    Node<T>* pLast = nullptr;

    while (pSrc && pDes)
    {
        pDes->data = pSrc->data;
		pLast = pDes;
        pDes = pDes->pNext;
        pSrc = pSrc->pNext;
    }

    // Source list is longer
    while (pSrc)
    {
        if (pLast)
            pLast = insert(pLast, pSrc->data, true);
        else
			pDestination = pLast = new Node<T>(pSrc->data);

		pSrc = pSrc->pNext;
    }

    // Destination list is longer
    if (pDes)
    {
        if (pLast)
            pLast->pNext = nullptr;
        else
			pDestination = nullptr;

		clear(pDes);
    }
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>*& pLHS, Node <T>*& pRHS)
{
	if (&pLHS != &pRHS)
    {
        Node<T>* pTemp = pLHS;
        pLHS = pRHS;
        pRHS = pTemp;
	}

}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T>* remove(const Node <T>* pRemove)
{
    if (!pRemove)
        return nullptr;

    Node <T>* pReturn;

    // If there is a node before pRemove, set their pNext to pRemove's pNext
    if (pRemove->pPrev)
        pRemove->pPrev->pNext = pRemove->pNext;

    // If there is a node after pRemove, set their pPrev to pRemove's pPrev
    if (pRemove->pNext)
        pRemove->pNext->pPrev = pRemove->pPrev;

    // If there is a node before pRemove, return a node pointing to that node. 
    // Otherwise, return a node pointing to the node following pRemove
    if (pRemove->pPrev)
        pReturn = pRemove->pPrev;
    else
        pReturn = pRemove->pNext;

    delete pRemove;
    return pReturn;
}


/**********************************************
 * INSERT
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T>* insert(Node <T>* pCurrent,
    const T& t,
    bool after = false)
{
    Node<T>* pNew = new Node<T>(t);

    // Inserting before pCurrent
    if ((pCurrent) && (!after))
    {
        // Connect pNew to pCurrent and pCurrent->pPrev without changing either node
        pNew->pNext = pCurrent;
        pNew->pPrev = pCurrent->pPrev;

        // Connect pCurrent to pNew
        pCurrent->pPrev = pNew;

        // If there is a node before pCurrent, connect it to pNew
        if (pNew->pPrev)
            pNew->pPrev->pNext = pNew;
    }

    // Inserting after pCurrent
    if ((pCurrent) && (after))
    {
        // Connect pNew to pCurrent and pCurrent->pNext without changing either node
        pNew->pPrev = pCurrent;
        pNew->pNext = pCurrent->pNext;

        // If there is a node after pCurrent, connect it to pNew
        if (pNew->pNext)
            pNew->pNext->pPrev = pNew;

        // Connect pCurrent to pNew
        pCurrent->pNext = pNew;
    }

    return pNew;
}

/******************************************************
 * SIZE
 * Find the size an unsorted linked list.
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : number of nodes
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T>* pHead)
{
    size_t size = 0;

    for (auto p = pHead; p; p = p->pNext)
        size++;

    return size;
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream& operator << (std::ostream& out, const Node <T>* pHead)
{
    return out;
}

/*****************************************************
 * FREE DATA
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T>*& pHead)
{
    while (pHead)
    {
        Node<T>* pDelete = pHead;
        pHead = pHead->pNext;
        delete pDelete;
    }
}
