/***********************************************************************
 * Header:
 *    BINARY NODE
 * Summary:
 *    One node in a binary tree (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BNode         : A class representing a BNode
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <iostream>  // for OFSTREAM
#include <cassert>

 /*****************************************************************
  * BNODE
  * A single node in a binary tree.  Note that the node does not know
  * anything about the properties of the tree so no validation can be done.
  *****************************************************************/
template <class T>
class BNode
{
public:
   // 
   // Construct
   //
   BNode()           : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data() {}
   BNode(const T& t) : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(t) {}
   BNode(T&& t)      : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(std::move(t)) {}

   //
   // Data
   //
   BNode <T>* pLeft;
   BNode <T>* pRight;
   BNode <T>* pParent;
   T data;
};

/*******************************************************************
 * SIZE BTREE
 * Return the size of a b-tree under the current node
 *******************************************************************/
template <class T>
inline size_t size(const BNode <T>* p)
{
   if (!p)
      return 0;
   else
      return size(p->pLeft) + 1 + size(p->pRight);
}


/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T>* pNode, BNode <T>* pAdd)
{
   assert(pNode != nullptr);

   pNode->pLeft = pAdd;

   if (pAdd != nullptr)
      pAdd->pParent = pNode;
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
inline void addRight(BNode <T>* pNode, BNode <T>* pAdd)
{
   assert(pNode != nullptr);

   pNode->pRight = pAdd;

   if (pAdd != nullptr)
      pAdd->pParent = pNode;
}

/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T>* pNode, const T& t)
{
   addLeft(pNode, new BNode<T>(t));
}

template <class T>
inline void addLeft(BNode <T>* pNode, T&& t)
{
   addLeft(pNode, new BNode<T>(std::move(t)));
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
void addRight(BNode <T>* pNode, const T& t)
{
   addRight(pNode, new BNode<T>(t));
}

template <class T>
void addRight(BNode <T>* pNode, T&& t)
{
   addRight(pNode, new BNode<T>(std::move(t)));
}

/*****************************************************
 * DELETE BINARY TREE
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/
template <class T>
void clear(BNode <T>*& pThis)
{
   if (pThis == nullptr)
      return;

   clear(pThis->pLeft);
   clear(pThis->pRight);

   delete pThis;
   pThis = nullptr;
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(BNode <T>*& pLHS, BNode <T>*& pRHS)
{
   if (pLHS == pRHS)
      return;
   BNode <T>* pTemp = pLHS;
   pLHS = pRHS;
   pRHS = pTemp;
}

/**********************************************
 * COPY BINARY TREE
 * Copy pSrc->pRight to pDest->pRight and
 * pSrc->pLeft onto pDest->pLeft
 *********************************************/
template <class T>
BNode <T>* copy(const BNode <T>* pSrc)
{
   if (!pSrc)
      return nullptr;

   BNode<T>* pNew = new BNode<T>(pSrc->data);

   pNew->pLeft = copy(pSrc->pLeft);
   if (pNew->pLeft)
      pNew->pLeft->pParent = pNew;

   pNew->pRight = copy(pSrc->pRight);
   if (pNew->pRight)
      pNew->pRight->pParent = pNew;

   return pNew;
}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <class T>
void assign(BNode <T>*& pDest, const BNode <T>* pSrc)
{
   if (!pSrc && !pDest)
      return;

   if (pSrc && !pDest)
   {
      pDest = copy(pSrc);
      return;
   }

   if (!pSrc && pDest)
   {
      clear(pDest);
      return;
   }

   pDest->data = pSrc->data;

   assign(pDest->pLeft, pSrc->pLeft);
   if (pDest->pLeft)
      pDest->pLeft->pParent = pDest;

   assign(pDest->pRight, pSrc->pRight);
   if (pDest->pRight)
      pDest->pRight->pParent = pDest;
}
