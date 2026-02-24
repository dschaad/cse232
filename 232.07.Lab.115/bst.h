/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    David Schaad, Tori Tremelling
 *    Time spent: 5 hours, 30 minutes
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>          // because I am paranoid
#include <memory>           // for std::allocator
#include <functional>       // for std::less
#include <utility>          // for std::pair
#include <initializer_list> // for the initializer list, of course!

class TestBST; // forward declaration for unit tests
class TestSet;
class TestMap;

namespace custom
{

    template <typename TT>
    class set;
    template <typename KK, typename VV>
    class map;

    /*****************************************************************
     * BINARY SEARCH TREE
     * Create a Binary Search Tree
     *****************************************************************/
    template <typename T>
    class BST
    {
        friend class ::TestBST; // give unit tests access to the privates
        friend class ::TestSet;
        friend class ::TestMap;

        template <class TT>
        friend class custom::set;

        template <class KK, class VV>
        friend class custom::map;
    public:
        //
        // Construct
        //

        BST();
        BST(const BST& rhs);
        BST(BST&& rhs);
        BST(const std::initializer_list<T>& il);
        ~BST();

        //
        // Assign
        //

        BST& operator = (const BST& rhs);
        BST& operator = (BST&& rhs);
        BST& operator = (const std::initializer_list<T>& il);
        void swap(BST& rhs);

        //
        // Iterator
        //

        class iterator;
        iterator   begin() const noexcept;
        iterator   end()   const noexcept { return iterator(nullptr); }

        //
        // Access
        //

        iterator find(const T& t);

        // 
        // Insert
        //

        std::pair<iterator, bool> insert(const T& t, bool keepUnique = false);
        std::pair<iterator, bool> insert(T&& t, bool keepUnique = false);

        //
        // Remove
        // 

        iterator erase(iterator& it);
        void   clear() noexcept;

        // 
        // Status
        //

        bool   empty() const noexcept { return size() == 0; } // unsure if it's better to compare against numElements directly or use size()
        size_t size()  const noexcept { return numElements; }

    private:

        class BNode;
        BNode* root;              // root node of the binary search tree
        size_t numElements;        // number of elements currently in the tree
    };


    /*****************************************************************
     * BINARY NODE
     * A single node in a binary tree. Note that the node does not know
     * anything about the properties of the tree so no validation can be done.
     *****************************************************************/
    template <typename T>
    class BST <T> ::BNode
    {
    public:
        // 
        // Construct
        //
        BNode() : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(), isRed(true) {}
        BNode(const T& t) : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(t), isRed(true) {}
        BNode(T&& t) : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(std::move(t)), isRed(true) {}

        //
        // Insert
        //
        void addLeft(BNode* pNode);
        void addRight(BNode* pNode);
        void addLeft(const T& t);
        void addRight(const T& t);
        void addLeft(T&& t);
        void addRight(T&& t);

        // 
        // Status
        //
        bool isRightChild(BNode* pNode) const { return pParent && pParent->pRight == this; }
        bool isLeftChild(BNode* pNode) const { return pParent && pParent->pLeft == this; }

        // balance the tree
        void balance();

#ifdef DEBUG
        //
        // Verify
        //
        std::pair <T, T> verifyBTree() const;
        int findDepth() const;
        bool verifyRedBlack(int depth) const;
        int computeSize() const;
#endif // DEBUG

        //
        // Data
        //
        T data;                  // Actual data stored in the BNode
        BNode* pLeft;            // Left child - smaller
        BNode* pRight;           // Right child - larger
        BNode* pParent;          // Parent
        bool isRed;              // Red-black balancing stuff
    };

    /**********************************************************
     * BINARY SEARCH TREE ITERATOR
     * Forward and reverse iterator through a BST
     *********************************************************/
    template <typename T>
    class BST <T> ::iterator
    {
        friend class ::TestBST; // give unit tests access to the privates
        friend class ::TestSet;
        friend class ::TestMap;

        template <class KK, class VV>
        friend class custom::map;
    public:
        // constructors and assignment
        iterator(BNode* p = nullptr) : pNode(p) {}
        iterator(const iterator& rhs) : pNode(rhs.pNode) {}

        iterator& operator = (const iterator& rhs)
        {
            pNode = rhs.pNode;
            return *this;
        }

        // compare
        bool operator == (const iterator& rhs) const
        {
            return pNode == rhs.pNode;
        }
        bool operator != (const iterator& rhs) const
        {
            return pNode != rhs.pNode;
        }

        // de-reference. Cannot change because it will invalidate the BST
        const T& operator * () const
        {
            return pNode->data;
        }

        // increment and decrement
        iterator& operator ++ ();
        iterator   operator ++ (int postfix)
        {
            BNode* oldNode = pNode;
            pNode += postfix;
            return oldNode;
        }
        iterator& operator -- ();
        iterator   operator -- (int postfix)
        {
            BNode* oldNode = pNode;
            pNode -= postfix;
            return oldNode;
        }

        // must give friend status to remove so it can call getNode() from it
        friend BST <T> ::iterator BST <T> ::erase(iterator& it);

    private:

        // the node
        BNode* pNode;
    };


    /*********************************************
     *********************************************
     *********************************************
     ******************** BST ********************
     *********************************************
     *********************************************
     *********************************************/


     /*********************************************
      * BST :: DEFAULT CONSTRUCTOR
      ********************************************/
    template <typename T>
    BST <T> ::BST() : root(nullptr), numElements(0) {}

    /*********************************************
     * BST :: COPY CONSTRUCTOR
     * Copy one tree to another
     ********************************************/
    template <typename T>
    BST <T> ::BST(const BST<T>& rhs) : root(nullptr), numElements(rhs.numElements)
    {
        if (!rhs.root)
            return;


        std::function<BNode* (BNode*, BNode*)> copy = [&](BNode* src, BNode* parent) -> BNode*
            {
                if (!src)
                    return nullptr;

                BNode* pNew = new BNode(src->data);
                pNew->isRed = src->isRed;
                pNew->pParent = parent;

                pNew->pLeft = copy(src->pLeft, pNew);
                pNew->pRight = copy(src->pRight, pNew);

                return pNew;
            };

        root = copy(rhs.root, nullptr);

    }

    /*********************************************
     * BST :: MOVE CONSTRUCTOR
     * Move one tree to another
     ********************************************/
    template <typename T>
    BST <T> ::BST(BST <T>&& rhs)
    {
        root = rhs.root;
        numElements = rhs.numElements;

        rhs.root = nullptr;
        rhs.numElements = 0;
    }

    /*********************************************
     * BST :: INITIALIZER LIST CONSTRUCTOR
     * Create a BST from an initializer list
     ********************************************/
    template <typename T>
    BST <T> ::BST(const std::initializer_list<T>& il)
    {
        for (const auto& item : il)
            insert(item);
    }

    /*********************************************
     * BST :: DESTRUCTOR
     ********************************************/
    template <typename T>
    BST <T> :: ~BST()
    {
        clear();
    }


    /*********************************************
     * BST :: ASSIGNMENT OPERATOR
     * Copy one tree to another
     ********************************************/
    template <typename T>
    BST <T>& BST <T> :: operator = (const BST <T>& rhs)
    {
        if (this == &rhs)
            return *this;

        std::function<BNode* (BNode*, BNode*)> clone =
            [&](BNode* src, BNode* parent) -> BNode*
            {
                if (!src)
                    return nullptr;

                BNode* n = new BNode(src->data);
                n->isRed = src->isRed;
                n->pParent = parent;
                n->pLeft = clone(src->pLeft, n);
                n->pRight = clone(src->pRight, n);
                return n;
            };

        // helper: delete subtree
        std::function<void(BNode*)> destroy =
            [&](BNode* n)
            {
                if (!n) return;
                destroy(n->pLeft);
                destroy(n->pRight);
                delete n;
            };

        // helper: assign while reusing nodes when possible
        std::function<void(BNode*&, BNode*, BNode*)> assign =
            [&](BNode*& dest, BNode* src, BNode* parent)
            {
                if (!src && !dest)
                    return;

                if (!src && dest)
                {
                    destroy(dest);
                    dest = nullptr;
                    return;
                }

                if (src && !dest)
                {
                    dest = clone(src, parent);
                    return;
                }

                // both exist reuse node
                dest->data = src->data;
                dest->isRed = src->isRed;
                dest->pParent = parent;

                assign(dest->pLeft, src->pLeft, dest);
                assign(dest->pRight, src->pRight, dest);
            };

        assign(root, rhs.root, nullptr);
        numElements = rhs.numElements;

        if (root)
            root->isRed = false;
        return *this;
    }

    /*********************************************
     * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
     * Copy nodes onto a BTree
     ********************************************/
    template <typename T>
    BST <T>& BST <T> :: operator = (const std::initializer_list<T>& il)
    {
        clear();

        for (const T& value : il)
            insert(value);

        return *this;
    }

    /*********************************************
     * BST :: ASSIGN-MOVE OPERATOR
     * Move one tree to another
     ********************************************/
    template <typename T>
    BST <T>& BST <T> :: operator = (BST <T>&& rhs)
    {
        if (this == &rhs)
            return *this;

        clear();

        root = rhs.root;
        numElements = rhs.numElements;

        rhs.root = nullptr;
        rhs.numElements = 0;

        return *this;
    }

    /*********************************************
     * BST :: SWAP
     * Swap two trees
     ********************************************/
    template <typename T>
    void BST <T> ::swap(BST <T>& rhs)
    {
        std::swap(this->root, rhs.root);
        std::swap(this->numElements, rhs.numElements);
    }

    /*****************************************************
     * BST :: INSERT
     * Insert a node at a given location in the tree
     ****************************************************/
    template <typename T>
    std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(const T& t, bool keepUnique)
    {
        bool done = false;
        BNode* currentNode = root;
        std::pair<iterator, bool> pairReturn(end(), false);

        // If there is no root node, create one
        if (!root)
        {
            root = new BNode(t);
            root->isRed = false;
            numElements = 1;
            pairReturn = std::pair<iterator, bool>(iterator(root), true);
            return pairReturn;
        }

        while (!done)
        {
            if (keepUnique)
            {
                // If we encounter a node with the same value, don't insert a new node and return the location of the existing node
                if (t == currentNode->data)
                {
                    pairReturn = std::pair<iterator, bool>(iterator(currentNode), false);
                    return pairReturn;
                }
            }

            if (t < currentNode->data)
            {
                // If currentNode has a left child, move to it
                if (currentNode->pLeft)
                    currentNode = currentNode->pLeft;

                // Otherwise, insert a new node as the current node's left child
                else
                {
                    currentNode->addLeft(t);
                    BNode* inserted = currentNode->pLeft;
                    numElements++;

                    pairReturn = std::pair<iterator, bool>(iterator(currentNode->pLeft), true);
                    done = true;
                }
            }

            else
            {
                // If currentNode has a right child, move to it
                if (currentNode->pRight)
                    currentNode = currentNode->pRight;

                // Otherwise, insert a new node as the current node's right child
                else
                {
                    currentNode->addRight(t);
                    BNode* inserted = currentNode->pRight;
                    numElements++;

                    pairReturn = std::pair<iterator, bool>(iterator(currentNode->pRight), true);
                    done = true;
                }
            }
        }

        // If we reshuffled the root node, locate and set the new root node
        while (root->pParent)
            root = root->pParent;

        if (root)
            root->isRed = false;

        return pairReturn;
    }

    template <typename T>
    std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T&& t, bool keepUnique) // unsure if there's a way to only need one insert defined
    {
        bool done = false;
        BNode* currentNode = root;
        std::pair<iterator, bool> pairReturn(end(), false);

        // If there is no root node, create one
        if (!root)
        {
            root = new BNode(std::move(t));
            root->isRed = false;
            numElements = 1;
            pairReturn = std::pair<iterator, bool>(iterator(root), true);
            return pairReturn;
        }

        while (!done)
        {
            if (keepUnique)
            {
                // If we encounter a node with the same value, don't insert a new node and return the location of the existing node
                if (t == currentNode->data)
                {
                    pairReturn = std::pair<iterator, bool>(iterator(currentNode), false);
                    return pairReturn;
                }
            }

            if (t < currentNode->data)
            {
                // If currentNode has a left child, move to it
                if (currentNode->pLeft)
                    currentNode = currentNode->pLeft;

                // Otherwise, insert a new node as the current node's left child
                else
                {
                    currentNode->addLeft(std::move(t));
                    numElements++;
                    pairReturn = std::pair<iterator, bool>(iterator(currentNode->pLeft), true);
                    done = true;
                }
            }

            else
            {
                // If currentNode has a right child, move to it
                if (currentNode->pRight)
                    currentNode = currentNode->pRight;

                // Otherwise, insert a new node as the current node's right child
                else
                {
                    currentNode->addRight(std::move(t));
                    numElements++;
                    pairReturn = std::pair<iterator, bool>(iterator(currentNode->pRight), true);
                    done = true;
                }
            }
        }

        // If we reshuffled the root node, locate and set the new root node
        while (root->pParent)
            root = root->pParent;

        return pairReturn;
    }

    /*************************************************
     * BST :: ERASE
     * Remove a given node as specified by the iterator
     ************************************************/
    template <typename T>
    typename BST<T>::iterator BST<T>::erase(iterator& it)
    {
        if (it == end())
            return end();

        BNode* node = it.pNode;

        // compute next iterator BEFORE structural change
        iterator itNext = ++it;

        // Case 1: No Children
        if (!node->pRight && !node->pLeft)
        {
            if (node->pParent && node->pParent->pRight == node)
                node->pParent->pRight = nullptr;
            if (node->pParent && node->pParent->pLeft == node)
                node->pParent->pLeft = nullptr;

            delete node;
            numElements--;
        }

        // Case 2: One Child
        else if (!node->pRight && node->pLeft)
        {
            node->pLeft->pParent = node->pParent;
            if (node->pParent && node->pParent->pRight == node)
                node->pParent->pRight = node->pLeft;
            if (node->pParent && node->pParent->pLeft == node)
                node->pParent->pLeft = node->pLeft;

            delete node;
            numElements--;
        }

        else if (!node->pLeft && node->pRight)
        {
            node->pRight->pParent = node->pParent;
            if (node->pParent && node->pParent->pRight == node)
                node->pParent->pRight = node->pRight;
            if (node->pParent && node->pParent->pLeft == node)
                node->pParent->pLeft = node->pRight;

            delete node;
            numElements--;
        }

        // Case 3: 2 Children
        else if (node->pLeft && node->pRight)
        {
            BNode* succ = itNext.pNode;

            // Detach the successor from its parent
            BNode* succParent = succ->pParent;
            BNode* succRight = succ->pRight;

            if (succParent != node)
            {
                if (succParent->pLeft == succ)
                    succParent->pLeft = succRight;
                else
                    succParent->pRight = succRight;

                if (succRight)
                    succRight->pParent = succParent;

                // successor takes node's right subtree
                succ->pRight = node->pRight;
                if (succ->pRight)
                    succ->pRight->pParent = succ;
            }

            // --- replace node with successor ---
            succ->pParent = node->pParent;

            if (!node->pParent)
                root = succ;
            else if (node->pParent->pLeft == node)
                node->pParent->pLeft = succ;
            else
                node->pParent->pRight = succ;

            // attach children
            succ->pLeft = node->pLeft;
            if (succ->pLeft)
                succ->pLeft->pParent = succ;

            delete node;
            numElements--;

            itNext = iterator(succ);
        }

        //while (root->pParent)
        //    root = root->pParent;

        return itNext;
    }

    /*****************************************************
     * BST :: CLEAR
     * Removes all the BNodes from a tree
     ****************************************************/
    template <typename T>
    void BST <T> ::clear() noexcept
    {
        std::function<void(BNode*)> destroy = [&](BNode* node)
            {
                if (!node)
                    return;

                destroy(node->pLeft);
                destroy(node->pRight);

                delete node;
            };

        destroy(root);
        root = nullptr;
        numElements = 0;
    }

    /*****************************************************
     * BST :: BEGIN
     * Return the first node (left-most) in a binary search tree
     ****************************************************/
    template <typename T>
    typename BST <T> ::iterator custom::BST <T> ::begin() const noexcept
    {
        if (empty())
            return end();

        BNode* p = root;

        while (p->pLeft)
            p = p->pLeft;

        return iterator(p);
    }


    /****************************************************
     * BST :: FIND
     * Return the node corresponding to a given value
     ****************************************************/
    template <typename T>
    typename BST <T> ::iterator BST<T> ::find(const T& t)
    {
        BNode* current = root;

        while (current)
        {
            if (t == current->data)
                return iterator(current);

            else if (t < current->data)
                current = current->pLeft;

            else
                current = current->pRight;
        }

        return end();
    }

    /******************************************************
     ******************************************************
     ******************************************************
     *********************** B NODE ***********************
     ******************************************************
     ******************************************************
     ******************************************************/


     /******************************************************
      * BINARY NODE :: ADD LEFT
      * Add a node to the left of the current node
      ******************************************************/
    template <typename T>
    void BST <T> ::BNode::addLeft(BNode* pNode)
    {
        this->pLeft = pNode;
        pNode->pParent = this;

        pNode->balance();
    }

    /******************************************************
     * BINARY NODE :: ADD RIGHT
     * Add a node to the right of the current node
     ******************************************************/
    template <typename T>
    void BST <T> ::BNode::addRight(BNode* pNode)
    {
        this->pRight = pNode;
        pNode->pParent = this;

        pNode->balance();
    }

    /******************************************************
     * BINARY NODE :: ADD LEFT
     * Add a node to the left of the current node
     ******************************************************/
    template <typename T>
    void BST<T> ::BNode::addLeft(const T& t)
    {
        addLeft(new BNode(t));
    }

    /******************************************************
     * BINARY NODE :: ADD LEFT
     * Add a node to the left of the current node
     ******************************************************/
    template <typename T>
    void BST<T> ::BNode::addLeft(T&& t)
    {
        addLeft(new BNode(std::move(t)));
    }

    /******************************************************
     * BINARY NODE :: ADD RIGHT
     * Add a node to the right of the current node
     ******************************************************/
    template <typename T>
    void BST <T> ::BNode::addRight(const T& t)
    {
        addRight(new BNode(t));
    }

    /******************************************************
     * BINARY NODE :: ADD RIGHT
     * Add a node to the right of the current node
     ******************************************************/
    template <typename T>
    void BST <T> ::BNode::addRight(T&& t)
    {
        addRight(new BNode(std::move(t)));
    }

#ifdef DEBUG
    /****************************************************
     * BINARY NODE :: FIND DEPTH
     * Find the depth of the black nodes. This is useful for
     * verifying that a given red-black tree is valid
     ****************************************************/
    template <typename T>
    int BST <T> ::BNode::findDepth() const
    {
        // if there are no children, the depth is ourselves
        if (pRight == nullptr && pLeft == nullptr)
            return (isRed ? 0 : 1);

        // if there is a right child, go that way
        if (pRight != nullptr)
            return (isRed ? 0 : 1) + pRight->findDepth();
        else
            return (isRed ? 0 : 1) + pLeft->findDepth();
    }

    /****************************************************
     * BINARY NODE :: VERIFY RED BLACK
     * Do all four red-black rules work here?
     ***************************************************/
    template <typename T>
    bool BST <T> ::BNode::verifyRedBlack(int depth) const
    {
        bool fReturn = true;
        depth -= (isRed == false) ? 1 : 0;

        // Rule a) Every node is either red or black
        assert(isRed == true || isRed == false); // this feels silly

        // Rule b) The root is black
        if (pParent == nullptr)
            if (isRed == true)
                fReturn = false;

        // Rule c) Red nodes have black children
        if (isRed == true)
        {
            if (pLeft != nullptr)
                if (pLeft->isRed == true)
                    fReturn = false;

            if (pRight != nullptr)
                if (pRight->isRed == true)
                    fReturn = false;
        }

        // Rule d) Every path from a leaf to the root has the same # of black nodes
        if (pLeft == nullptr && pRight && nullptr)
            if (depth != 0)
                fReturn = false;
        if (pLeft != nullptr)
            if (!pLeft->verifyRedBlack(depth))
                fReturn = false;
        if (pRight != nullptr)
            if (!pRight->verifyRedBlack(depth))
                fReturn = false;

        return fReturn;
    }


    /******************************************************
     * VERIFY B TREE
     * Verify that the tree is correctly formed
     ******************************************************/
    template <typename T>
    std::pair <T, T> BST <T> ::BNode::verifyBTree() const
    {
        // largest and smallest values
        std::pair <T, T> extremes;
        extremes.first = data;
        extremes.second = data;

        // check parent
        if (pParent)
            assert(pParent->pLeft == this || pParent->pRight == this);

        // check left, the smaller sub-tree
        if (pLeft)
        {
            assert(!(data < pLeft->data));
            assert(pLeft->pParent == this);
            pLeft->verifyBTree();
            std::pair <T, T> p = pLeft->verifyBTree();
            assert(!(data < p.second));
            extremes.first = p.first;

        }

        // check right
        if (pRight)
        {
            assert(!(pRight->data < data));
            assert(pRight->pParent == this);
            pRight->verifyBTree();

            std::pair <T, T> p = pRight->verifyBTree();
            assert(!(p.first < data));
            extremes.second = p.second;
        }

        // return answer
        return extremes;
    }

    /*********************************************
     * COMPUTE SIZE
     * Verify that the BST is as large as we think it is
     ********************************************/
    template <typename T>
    int BST <T> ::BNode::computeSize() const
    {
        return 1 +
            (pLeft == nullptr ? 0 : pLeft->computeSize()) +
            (pRight == nullptr ? 0 : pRight->computeSize());
    }
#endif // DEBUG

    /******************************************************
     * BINARY NODE :: BALANCE
     * Balance the tree from a given location
     ******************************************************/
    template <typename T>
    void BST <T> ::BNode::balance()
    {
        BNode* node = this;

        // Case 1: if we are the root, then color ourselves black and call it a day.
        if (!node->pParent)
        {
            node->isRed = false;
            return;
        }

        // Case 2: if the parent is black, then there is nothing left to do
        if (!node->pParent->isRed)
            return;

        BNode* parent = node->pParent;
        BNode* grandparent = parent->pParent;

        if (!grandparent)
            return;

        BNode* aunt = (grandparent->pLeft == parent) ? grandparent->pRight : grandparent->pLeft;

        // Case 3: if the aunt is red, then just recolor
        if (aunt && aunt->isRed)
        {
            parent->isRed = false;
            aunt->isRed = false;
            grandparent->isRed = true;
            grandparent->balance();
            return;
        }

        // Case 4: if the aunt is black or non-existant, then we need to rotate

        // Case 4a: We are mom's left and mom is granny's left
        if (parent == grandparent->pLeft && node == parent->pLeft)
        {
            parent->isRed = false;
            grandparent->isRed = true;

            grandparent->pLeft = parent->pRight;
            if (parent->pRight)
                parent->pRight->pParent = grandparent;

            parent->pRight = grandparent;
            parent->pParent = grandparent->pParent;

            if (grandparent->pParent)
            {
                if (grandparent->pParent->pLeft == grandparent)
                    grandparent->pParent->pLeft = parent;
                else
                    grandparent->pParent->pRight = parent;
            }
            else
            {
                parent->pParent = nullptr;
            }

            grandparent->pParent = parent;
        }

        // Case 4b: right-right
        else if (parent == grandparent->pRight && node == parent->pRight)
        {
            parent->isRed = false;
            grandparent->isRed = true;

            grandparent->pRight = parent->pLeft;
            if (parent->pLeft)
                parent->pLeft->pParent = grandparent;

            parent->pLeft = grandparent;
            parent->pParent = grandparent->pParent;

            if (grandparent->pParent)
            {
                if (grandparent->pParent->pLeft == grandparent)
                    grandparent->pParent->pLeft = parent;
                else
                    grandparent->pParent->pRight = parent;
            }
            else
            {
                parent->pParent = nullptr;
            }

            grandparent->pParent = parent;
        }

        // Case 4c: left-right
        else if (parent == grandparent->pLeft && node == parent->pRight)
        {
            node->isRed = false;
            grandparent->isRed = true;

            parent->pRight = node->pLeft;
            if (node->pLeft)
                node->pLeft->pParent = parent;

            grandparent->pLeft = node->pRight;
            if (node->pRight)
                node->pRight->pParent = grandparent;

            node->pLeft = parent;
            node->pRight = grandparent;

            node->pParent = grandparent->pParent;

            if (grandparent->pParent)
            {
                if (grandparent->pParent->pLeft == grandparent)
                    grandparent->pParent->pLeft = node;
                else
                    grandparent->pParent->pRight = node;
            }
            else
            {
                node->pParent = nullptr;
            }

            parent->pParent = node;
            grandparent->pParent = node;
        }

        // Case 4d: right-left
        else if (parent == grandparent->pRight && node == parent->pLeft)
        {
            node->isRed = false;
            grandparent->isRed = true;

            parent->pLeft = node->pRight;
            if (node->pRight)
                node->pRight->pParent = parent;

            grandparent->pRight = node->pLeft;
            if (node->pLeft)
                node->pLeft->pParent = grandparent;

            node->pRight = parent;
            node->pLeft = grandparent;

            node->pParent = grandparent->pParent;

            if (grandparent->pParent)
            {
                if (grandparent->pParent->pLeft == grandparent)
                    grandparent->pParent->pLeft = node;
                else
                    grandparent->pParent->pRight = node;
            }
            else
            {
                node->pParent = nullptr;
            }

            parent->pParent = node;
            grandparent->pParent = node;
        }

        // ensure root is black
        BNode* r = node;
        while (r->pParent)
            r = r->pParent;
        r->isRed = false;
    }

    /*************************************************
     *************************************************
     *************************************************
     ****************** ITERATOR *********************
     *************************************************
     *************************************************
     *************************************************/

     /**************************************************
      * BST ITERATOR :: INCREMENT PREFIX
      * advance by one
      *************************************************/
    template <typename T>
    typename BST <T> ::iterator& BST <T> ::iterator :: operator ++ ()
    {
        // Can't increment from a null node
        if (!pNode)
            ;

        // Case 1: Have a right child
        else if (pNode->pRight)
        {
            // Go to our right child, then go as far left as possible
            pNode = pNode->pRight;
            while (pNode->pLeft)
                pNode = pNode->pLeft;
        }

        // Case 2: No right child and we are our parent's left child
        else if (!pNode->pRight && pNode->pParent->pLeft == pNode)
        {
            // Go to our parent node
            pNode = pNode->pParent;
        }

        // Case 3: No right child and we are our parent's right child
        else if (!pNode->pRight && pNode->pParent->pRight == pNode)
        {
            // Go up to our parent nodes until we're no longer a right child, then go to our parent
            while (pNode->pParent && pNode->pParent->pRight == pNode)
                pNode = pNode->pParent;

            pNode = pNode->pParent;
        }

        return *this;
    }

    /**************************************************
     * BST ITERATOR :: DECREMENT PREFIX
     * advance by one
     *************************************************/
    template <typename T>
    typename BST <T> ::iterator& BST <T> ::iterator :: operator -- ()
    {
        // Can't increment from a null node
        if (!pNode)
            ;

        // Case 1: Have a left child
        else if (pNode->pLeft)
        {
            // Go to our left child, then go as far right as possible
            pNode = pNode->pLeft;
            while (pNode->pRight)
                pNode = pNode->pRight;
        }

        // Case 2: No left child and we are our parent's right child
        else if (!pNode->pLeft && pNode->pParent->pRight == pNode)
        {
            // Go to our parent node
            pNode = pNode->pParent;
        }

        // Case 3: No left child and we are our parent's left child
        else if (!pNode->pLeft && pNode->pParent->pLeft == pNode)
        {
            // Go up to our parent nodes until we're no longer a left child, then go to our parent
            while (pNode->pParent && pNode->pParent->pLeft == pNode)
                pNode = pNode->pParent;

            pNode = pNode->pParent;
        }

        return *this;

    }


} // namespace custom
