/*
Author: Julian Lombardi
Class: CSI-281-04
Assignment: PA7
Date Assigned: 11/17/17
Due Date: 12/1/17 11:00am
Description:
Creates a self balancing tree that handles insertion and deletion while retaining AVL properties
Certification of Authenticity:
I certify that this is entirely my own work, except where I have given
fully-documented references to the work of others. I understand the
definition and consequences of plagiarism and acknowledge that the assessor
of this assignment may, for the purpose of assessing this assignment:
- Reproduce this assignment and provide a copy to another member of
academic staff; and/or
- Communicate a copy of this assignment to a plagiarism checking
service (which may then retain a copy of this assignment on its
database for the purpose of future plagiarism checking)
*/

#ifndef _AVL_H
#define _AVL_H

#include <iostream>
#include "queue.h"

using namespace std;

template <typename T>
class AVL
{
public:
    AVL();
    ~AVL();


    // many of these are wrapper functions for the recursive private versions below
    bool bfs(T searchKey);
    bool dfs(T searchKey);
    int  getHeight();
    void insert(T data);
    bool isEmpty();
    bool isInTree(T searchKey);
    int  leavesCount();
    int  nodesCount();
    void remove(T searchKey);
    void printPath(T searchKey);
    void showInOrder();
    void showPreOrder();
    void showPostOrder();
    void showTree();

    void rotateRight();
    void rotateLeft();

private:

    struct Node
    {
        T           mData;
        Node *mLeft, *mRight;

        //Pre:      None
        //Post      This object is initialized using default values
        //Purpose:  To intialize node object
        Node()
        {
            mData = T();
            mLeft = NULL;
            mRight = NULL;
        }

        //Pre:      None
        //Post:     This object is initialized using specified data
        //Purpose:  To intialize node object
        Node(T data)
        {
            mData = data;
            mLeft = NULL;
            mRight = NULL;
        }

        //Pre:      None
        //Post:     This object is initialized using specified data with the
        //          specified children attached
        //Purpose:  To intialize node object
        Node(T data, Node *left, Node *right)
        {
            mData = data;
            mLeft = left;
            mRight = right;
        }
    };

    Node *mRootNode;  // the root of the tree

    AVL(T data, Node *left, Node *right);   //Constructor


    bool bfs(Queue<Node*> *queue, const T &searchKey);
    void destroySubtree(Node *node);
    bool dfs(Node *node, const T &searchKey);
    void displayInOrder(Node *node);
    void displayPreOrder(Node *node);
    void displayPostOrder(Node *node);
    void displayTree(Node *node, int tab);
    void getHeight(Node *node, int &max, int layer);
    void insert(Node *&node, const T &data);        
    void leavesCount(Node *node, int &count);
    void nodesCount(Node *node, int &count);
    void performDeletion(Node *&node);
    void remove(Node *&node, const T &searchKey);   

    //MY FUNCTIONS
    int getHeight(Node *node);
    int getBalance(Node *&node);

    void rightRotate(Node *&node);
    void leftRotate(Node *&node);


};

//PRE:      None
//POST:     Creates a head node for the tree with NULL data
//PURPOSE:  Default constructor
template <typename T>
AVL<T>::AVL()
{
    mRootNode = NULL;
}

//PRE:      Requires data to be inserted into the node, and the nodes on the left and right of it
//POST:     Creates a node with specified data pointing to the left and right nodes
//PURPOSE:  Constructor for when given data and the left & right nodes
template <typename T>
AVL<T>::AVL(T data, Node *left, Node *right)
{
    Node *newNode;

    newNode = new Node(data, left, right);
    if (newNode != NULL)
        mRootNode = newNode;
}

//PRE:      None
//POST:     Destroys entire tree
//PURPOSE:  Default destructor for AVL
template <typename T>
AVL<T>::~AVL()
{
    destroySubtree(mRootNode);
}

//PRE:      Requires a key to be searched for
//POST:     Returns true of value is found, false if not found
//PURPOSE:  Uses Breadth first search to look through a tree to find a node
//          This is the wrapper function for the following bfs.
template <typename T>
bool AVL<T>::bfs(T searchKey)
{
    bool found = false;
    Queue<Node*> *queue = new Queue<Node*>;

    if (!isEmpty())
    {
        queue->enqueue(mRootNode);
        found = bfs(queue, searchKey);
    }

    delete queue;

    return found;
}

//PRE:      Requires a to begin the search from, and a key to search for
//POST:     Returns true of the value is found, false if not found
//PURPOSE:  Uses depth first search to look through the tree and find a node
template <typename T>
bool AVL<T>::bfs(Queue<Node*> *queue, const T &searchKey)
{
    bool found = false;
    Node* tmp;

    if (!queue->isEmpty())
    {
        tmp = queue->dequeue();


        if (tmp->mData == searchKey)
            found = true;
        else
        {
            if (tmp->mLeft != NULL)
                queue->enqueue(tmp->mLeft);

            if (tmp->mRight != NULL)
                queue->enqueue(tmp->mRight);

            found = bfs(queue, searchKey);
        }

    }

    return found;
}

//PRE:      Requires a node to destroy all following subtrees of
//POST:     All sub trees of node are deleted
//PURPOSE:  Destroys all sub trees spanning off of a specified node
template <typename T>
void AVL<T>::destroySubtree(Node *node)
{
    if (node == NULL)
        return;

    destroySubtree(node->mLeft);
    destroySubtree(node->mRight);

    // Delete the node at the root.
    delete node;
}

//PRE:      Requires a key to search for
//POST:     Returns true of the value is found, false if not found
//PURPOSE:  Uses depth first search to look through the tree and find a node
//          This is the wrapper function for the following dfs
template <typename T>
bool AVL<T>::dfs(T searchKey)
{
    bool found = false;

    if (!isEmpty())
        found = dfs(mRootNode, searchKey);

    return found;
}


//PRE:      Requires a node to begin the search from, and a key to search for
//POST:     Returns true of the value is found, false if not found
//PURPOSE:  Uses depth first search to look through the tree and find a node
template <typename T>
bool AVL<T>::dfs(Node *node, const T &searchKey)
{
    bool found = false;

    if (node != NULL)
    {

        if (node->mData == searchKey)
            found = true;
        else
        {
            found = dfs(node->mLeft, searchKey);

            if (!found)
                found = dfs(node->mRight, searchKey);
        }

    }

    return found;
}

//PRE:      Requires a node to print from
//POST:     
//PURPOSE:  Prints out the nodes traversed when using in order
template <typename T>
void AVL<T>::displayInOrder(Node *node)
{
    if (node != NULL)
    {
        displayInOrder(node->mLeft);
        cout << node->mData << "  ";
        displayInOrder(node->mRight);
    }
}

//PRE:      Requires a node to print from
//POST:     
//PURPOSE:  Prints out the nodes traversed when using pre order
template <typename T>
void AVL<T>::displayPreOrder(Node *node)
{
    if (node != NULL)
    {
        cout << node->mData << "  ";
        displayPreOrder(node->mLeft);
        displayPreOrder(node->mRight);
    }
}

//PRE:      Requires a node to print from
//POST:     
//PURPOSE:  Prints out the nodes traversed when using post order
template <typename T>
void AVL<T>::displayPostOrder(Node *node)
{
    if (node != NULL)
    {
        displayPostOrder(node->mLeft);
        displayPostOrder(node->mRight);
        cout << node->mData << "  ";
    }
}

//PRE:      Requires a noce to print from, and the desired sizing of tab to display
//POST: 
//PURPOSE:  Prints out the tree in an understandable, human readable, way
template <typename T>
void AVL<T>::displayTree(Node *node, int tab)
{
    int i;

    if (node != NULL)
    {
        for (i = 0; i < tab; i++)
            cout << "  ";

        cout << node->mData << endl;

        displayTree(node->mLeft, tab + 1);
        displayTree(node->mRight, tab + 1);

    }
}

//PRE:      None
//POST:     Returns the height of the entire tree
//PURPOSE:  Finds the height of the entire tree
template <typename T>
int AVL<T>::getHeight()
{
    int max = 0;

    getHeight(mRootNode, max, 0);
    return max;
}

//THIS OVERLOAD USED TO GET HEIGHT OF SPECIFIC NODE
//PRE:      A node must be passed into the function
//POST:     Returns the height of the node
//PURPOSE:  Finds the height of a specific node (rather than only the root)
template <typename T>
int AVL<T>::getHeight(Node *node)
{
    int max = 0;

    getHeight(node, max, 0);
    return max;
}

//PRE:      Requires a node be passed to it
//POST:     Returns the balance factor of that node
//PURPOSE:  Finds the balance factor of a node in order to perform rotations
template<typename T>
inline int AVL<T>::getBalance(Node *&node)
{

    int leftHeight, rightHeight, balance;

    leftHeight = getHeight(node->mLeft);
    rightHeight = getHeight(node->mRight);

    balance = leftHeight - rightHeight;

    return balance;
}

//PRE:      Requires a node
//POST:     A right rotation will be performed on the node passed to it
//PURPOSE:  Rotates a node right to balance a tree
template<typename T>
inline void AVL<T>::rightRotate(Node *&node)
{
    Node *tempLeft = node->mLeft;
    Node *tempRight = tempLeft->mRight;

    tempLeft->mRight = node;
    node->mLeft = tempRight;

    node = tempLeft;
}
//PRE:      Requires a node
//POST:     A left rotation will be performed on the node passed to it
//PURPOSE:  Rotates a node left to balance a tree
template<typename T>
inline void AVL<T>::leftRotate(Node *&node)
{
    Node *tempRight = node->mRight;
    Node *tempLeft = tempRight->mLeft;

    tempRight->mLeft = node;
    node->mRight = tempLeft; 

    node = tempRight;
}

//PRE:      Requires a node to find the height of, a running count of the max level found, and what layer currently on
//POST:     Returns the height of the node
//PURPOSE:  Finds the height of a specified node 
template <typename T>
void AVL<T>::getHeight(Node *node, int &max, int layer)
{
    if (node == NULL)
        return;

    if (node->mLeft == NULL && node->mRight == NULL)
    {
        if (layer > max)
            max = layer;
    }
    else
    {
        layer++;
        getHeight(node->mLeft, max, layer);
        getHeight(node->mRight, max, layer);
    }
}

//PRE:      Requires the data to be inserted into a node
//POST:     Node will be inserted with specified data and tree will be balanced afterwards
//PURPOSE:  To insert a node into a tree and have it remain balanced afterwards.
//          This is a wrapper function to the following insert.
template <typename T>
void AVL<T>::insert(T data)
{
    insert(mRootNode, data);
}

//PRE:      Requires a node and the data to be inserted into it
//POST:     Node will be inserted with specified data and tree will be balanced afterwards
//PURPOSE:  To insert a node into a tree and have it remain balanced afterwards
template <typename T>
void AVL<T>::insert(Node *&node, const T &data)
{
    // If the tree is empty, make a new node and make it 
    // the root of the tree.
    if (node == NULL)
    {
        node = new Node(data);
        return;
    }

    // If num is already in tree: return.
    if (node->mData == data)
        return;

    // The tree is not empty: insert the new node into the
    // left or right subtree.
    if (data < node->mData)
        insert(node->mLeft, data);
    else
        insert(node->mRight, data);


    //BALANCE TREE
    int bFactor = getBalance(node); //balance factor

    //Right rotation
    if(bFactor > 1 && data < node->mLeft->mData){
        rightRotate(node);
        return;
    }

    //Left rotation
    if(bFactor < -1 && data > node->mRight->mData) {
        leftRotate(node);
        return;
    }

    //Left-right rotation
    if (bFactor > 1 && data > node->mLeft->mData) {
        leftRotate(node->mLeft);
        rightRotate(node);
        return;
    }

    //Right-left rotation
    if (bFactor < -1 && data < node->mRight->mData) {
        rightRotate(node->mRight);
        leftRotate(node);
        return;
    }
}

//PRE:      Node
//POST:     Returns true if the tree is empty, false if not empty
//PURPOSE:  Determine wether or not the tree is empty
template <typename T>
bool AVL<T>::isEmpty()
{
    return (mRootNode == NULL);
}

//PRE:      A key to search for
//POST:     Returns true if value is found, false if it is not
//PURPOSE:  Determine wether or not a specified value is in the tree
template <typename T>
bool AVL<T>::isInTree(T searchKey)
{
    Node *tmp = mRootNode;

    while (tmp != NULL)
    {
        if (tmp->mData == searchKey)
            return true;
        else if (tmp->mData > searchKey)
            tmp = tmp->mLeft;
        else
            tmp = tmp->mRight;
    }
    return false;
}

//PRE:      None
//POST:     A count of the number of leaves in a tree
//PURPOSE:  Finds the total number of leaves in the tree
//          This is a wrapper function for the following leavescount
template <typename T>
int AVL<T>::leavesCount()
{
    int count = 0;
    leavesCount(mRootNode, count);
    return count;
}

//PRE:      A node to start from, and the current count of leaves
//POST:     A count of the number of leaves in a tree
//PURPOSE:  Finds the total number of leaves in the tree
template <typename T>
void AVL<T>::leavesCount(Node *node, int &count)
{
    if (node == NULL)
        return;

    if (node->mLeft == NULL && node->mRight == NULL)
        count++;
    else
    {
        leavesCount(node->mLeft, count);
        leavesCount(node->mRight, count);
    }
}

//PRE:      Requires a specified node to delete
//POST:     The node will be destroyed and subtrees reattached
//PURPOSE:  Deletes a specified node and reattaches the subtrees as needed
template <typename T>
void AVL<T>::performDeletion(Node *&node)
{
    Node *nodeToDelete = node;

    Node *attachPoint;

    if (node->mRight == NULL)
    {
        // Replace tree with its left subtree. 
        node = node->mLeft;
    }
    else if (node->mLeft == NULL)
    {
        // Replace tree with its right subtree.
        node = node->mRight;
    }
    else
      
    {
        attachPoint = node->mRight;

        while (attachPoint->mLeft != NULL)
            attachPoint = attachPoint->mLeft;

        attachPoint->mLeft = node->mLeft;

        node = node->mRight;
    }

    delete nodeToDelete;
}

//PRE:      None
//POST:     Returns the number of nodes that were counted
//PURPOSE:  Counts the total number of nodes in the tree
//          This is also a wrapper function to the following nodesCount
template <typename T>
int AVL<T>::nodesCount()
{
    int count = 0;
    nodesCount(mRootNode, count);

    return count;
}

//PRE:      The node to count from, and the running total of the count
//POST:     Returns the number of nodes counted
//PURPOSE:  Counts the total number of nodes in the tree
template <typename T>
void AVL<T>::nodesCount(Node *node, int &count)
{
    if (node != NULL)
    {
        nodesCount(node->mLeft, count);
        nodesCount(node->mRight, count);
        count++;
    }

}

//PRE:      A key to search for
//POST:     Prints out the path taken to the key
//PURPOSE:  Traverses through tree looking for a specified value
//          Printing out every value visited along the way
template <typename T>
void AVL<T>::printPath(T searchKey)
{
    Node *tmp = mRootNode;
    Queue<T> queue;

    bool        found = false;

    while (tmp != NULL && !found)
    {
        queue.enqueue(tmp->mData);

        if (tmp->mData == searchKey)
            found = true;
        else if (tmp->mData > searchKey)
            tmp = tmp->mLeft;
        else
            tmp = tmp->mRight;
    }

    if (!found)
        cout << searchKey << " is not found in the list\n";
    else
    {
        cout << "Path: ";
        queue.display();
        cout << endl;
    }
}

//PRE:      Requires a node be passed to it and a search key
//POST:     Specified node will be deleted and tree will be rebalanced
//PURPOSE:  Removes a node from the tree and retains the balance of the tree
template <typename T>
void AVL<T>::remove(Node *&node, const T &searchKey)
{
    if (node == NULL)
        return;
    else if (node->mData > searchKey)
        remove(node->mLeft, searchKey);
    else if (node->mData < searchKey)
        remove(node->mRight, searchKey);
    else
        // We have found the node to delete.
        performDeletion(node);

    //BALANCE TREE
    int bFactor = getBalance(mRootNode);
    int rootLeftBalance = getBalance(mRootNode->mLeft);
    int rootRightBalance = getBalance(mRootNode->mRight);

    //Right rotation
    if (bFactor > 1 && rootLeftBalance >= 0) {
        rightRotate(mRootNode);
        return;
    }

    //Left rotation
    if (bFactor < -1 && rootRightBalance <= 0) {
        leftRotate(mRootNode);
        return;
    }

    //Left-right rotation
    if (bFactor > 1 && rootLeftBalance < 0) {
        leftRotate(mRootNode->mLeft);
        rightRotate(mRootNode);
        return;
    }

    //Right-left rotation
    if (bFactor < -1 && rootRightBalance > 0) {
        rightRotate(mRootNode->mRight);
        leftRotate(mRootNode);
        return;
    }
}


//The following are all wrapper functions to for their specified action/function name
//These all simply start the operation from the root node.
template <typename T>
void AVL<T>::remove(T searchKey)
{
    remove(mRootNode, searchKey);
}


template <typename T>
void AVL<T>::showInOrder()
{
    displayInOrder(mRootNode);
}


template <typename T>
void AVL<T>::showPreOrder()
{
    displayPreOrder(mRootNode);
}


template <typename T>
void AVL<T>::showPostOrder()
{
    displayPostOrder(mRootNode);
}


template <typename T>
void AVL<T>::showTree()
{
    displayTree(mRootNode, 0);
}

template<typename T>
inline void AVL<T>::rotateRight()
{
    rightRotate(mRootNode);
}

template<typename T>
inline void AVL<T>::rotateLeft()
{
    leftRotate(mRootNode);
}

#endif