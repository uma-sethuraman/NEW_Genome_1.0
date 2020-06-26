/**
 * \file SegmentTree.cpp
 * \author Victoria Cao
 **/


#include <cstdint>

#include "SegmentTree.h"


/*********************************************************************************
 *  PRIVATE
 * 
 *  Helper functions for getting and updating nodes
 * 
 * 
 ********************************************************************************/


/** Gets height of node
 * \param node to get height of
 * \return height of node of -1 if null
 **/
int SegmentTree::GetHeight(SegmentNode* node)
{
    if (node)
        return node->Height;
    else
        return -1;
    
}

/** Calculates the balance factor of node
 * \param node to get the balance factor of 
 * \return balance factor of the node
 **/
int SegmentTree::GetBalance(SegmentNode* node)
{
    auto left = GetHeight(node->Left);
    auto right = GetHeight(node->Right);

    return left - right;
}

/** Updates height of Node
 * \param node to update height of
 **/
void SegmentTree::UpdateHeight(SegmentNode* node)
{
    if (node)
    {   
        int left = GetHeight(node->Left);
        int right = GetHeight(node->Right);
        
        // node height is one plus the taller subtree
        node->Height = std::max(left, right) +1;
    }
}

/** Updates weight of node
 * \param node to update weight of
 **/
void SegmentTree::UpdateWeight(SegmentNode* node)
{
    if (node)
    {   
        size_t right = 0;
        size_t left = 0;
        if (node->Right)
            right = node->Right->Weight;
        if (node->Left)
            left = node->Left->Weight;

        // node weight is sum of left and right subtrees and it's own weight
        node->Weight = node->Size + right + left;
    }
}

/** Updates the tree from node to root
 * \param node to start updating from
 **/
void SegmentTree::Update(SegmentNode* node)
{
    while (node)
    {
        // update weight and height of node
        UpdateWeight(node);
        UpdateHeight(node);

        // rebalance if necessary and move up the tree
        node = ReBalance(node)->Parent;
    }
}

/** Creates a new node in the pool 
 * \param data creates new segment in tree
 * \return poitner to new node
 **/
template < typename T >
SegmentNode* SegmentTree::CreateNode(T data)
{
    return Pool->CreateNode(std::make_shared< GeneSegment >(data));
}


/*********************************************************************************
 *  PRIVATE
 * 
 *  Rebalancing functions
 * 
 * 
 ********************************************************************************/


/** Rotates subtree right
 * \param root root of subtree to rotate on
 * \return new root of the subtree
 **/
SegmentNode* SegmentTree::RotateRight(SegmentNode* root)
{
    SegmentNode* newRoot = root->Left; // new newRoot 
    SegmentNode* rootAdopt = newRoot->Right;

    if (root->Parent)   /// change root parent
    {
        if (root->Parent->Left == root)
        {
            newRoot->Parent = root->Parent;
            newRoot->Parent->Left = newRoot;
        }
        else
        {
            newRoot->Parent = root->Parent;
            newRoot->Parent->Right = newRoot;
        }
    }
    else    /// changing newRoot 
    {
        newRoot->Parent = nullptr;
        Root = newRoot;
    }

    // change root and new root
    root->Parent = newRoot;
    newRoot->Right = root;

    // change root and root adopt
    root->Left = rootAdopt;
    if (rootAdopt)
        rootAdopt->Parent = root;

    // update heights and weight
    UpdateHeight(root);
    UpdateHeight(newRoot);

    UpdateWeight(root);
    UpdateWeight(newRoot);  

    return newRoot;
}

/** Rotates subtree left
 * \param root root of subtree to rotate on
 * \return new root of the subtree
 **/
SegmentNode* SegmentTree::RotateLeft(SegmentNode* root)
{
    SegmentNode* newRoot = root->Right; // new newRoot 
    SegmentNode* rootAdopt = newRoot->Left;

    if (root->Parent)   /// change root parent
    {
        if (root->Parent->Left == root)
        {
            newRoot->Parent = root->Parent;
            newRoot->Parent->Left = newRoot;
        }
        else
        {
            newRoot->Parent = root->Parent;
            newRoot->Parent->Right = newRoot;
        }
    }
    else    /// changing newRoot 
    {
        newRoot->Parent = nullptr;
        Root = newRoot;
    }

    // change root and new root
    root->Parent = newRoot;
    newRoot->Left = root;

    // change root and root adopt
    root->Right = rootAdopt;
    if (rootAdopt)
        rootAdopt->Parent = root;

    // update heights and weight
    UpdateHeight(root);
    UpdateHeight(newRoot);

    UpdateWeight(root);
    UpdateWeight(newRoot);    

    return newRoot;
}



/** Rebalances at the node if needed
 * \param node to check for rebalancing
 * \return new node that replaces node if rebalanced
 **/
SegmentNode* SegmentTree::ReBalance(SegmentNode* node)
{
    auto balance = GetBalance(node); // Balance factor for node

    if (balance == 2) // Right case
    {
        if (GetBalance(node->Left) == -1) // Right Left case
            RotateLeft(node->Left);
        return RotateRight(node);
    }
    else if (balance == -2) // Left case
    {
        if (GetBalance(node->Right) == 1) // Left Right case
            RotateRight(node->Right);
        return RotateLeft(node);
    }

    return node;
}



/*********************************************************************************
 *  PUBLIC
 * 
 *  Mutation functions
 * 
 * 
 ********************************************************************************/


/** Finds the index in the tree
 * \param index to find
 * \return node that the index lies within, local index into the segment
 * that the index is at
 **/
std::pair<SegmentNode*, size_t> SegmentTree::Find(size_t index)
{
    SegmentNode* node = Root;

    /// boundary variables
    size_t left = 0;    // Left boundary of node
    size_t right = node->Weight;    // Right boundary of node
    if (node->Left)
        left += node->Left->Weight;
    if (node->Right)
        right -= node->Right->Weight;

    size_t min = 0;     // Left boundary of node's left subtree
    size_t max = node->Weight;  // Right boundary of node's right subtree
    
    /// Find the segment that index lies within
    while(index < left || index >= right)
    {
        if (index >= right)
        { 
            min = right;
            node = node->Right;
        }  
        else
        {
            max = left;
            node = node->Right;
        }
        
        // Update node's segment boundary
        left = min;
        right = max;
        if (node->Left)
            left += node->Left->Weight;
        if (node->Right)
            right -= node->Right->Weight;
           
    }

    return {node, left};
}


/** Deletes site at index in the tree
 * \param index index to delete site
 **/
void SegmentTree::Delete(size_t index)
{
    /// cut up the node at the index
    auto found = Find(index);
    auto node = found.first;
    auto startIndex = found.second;

    SegmentNode* cutNode = node->Cut(index-startIndex);
    cutNode->TruncateLeft();    // remove value at index

    /// adopt nodes right
    cutNode->Right = node->Right;
    if (cutNode->Right)
        cutNode->Right->Parent = cutNode;

    /// change node right to cut node
    node->Right = cutNode;
    cutNode->Parent = node;

    /// update the tree
    Update(cutNode);
    Size += 1;
}

/** Inserts mutation into index in the tree
 * \param mutation new mutation to insert
 * \param index index to insert the mutation after 
 **/
template < typename T >
void SegmentTree::Insert(size_t index, T mutation)
{
    /// create new node from mutation
    SegmentNode* newNode = CreateNode(mutation);

    /// cut up the node at the index
    auto found = Find(index);
    auto node = found.first;
    auto startIndex = found.second;

    SegmentNode* cutNode = node->Cut(index-startIndex);

    /// adopt nodes right
    cutNode->Right = node->Right;
    if (cutNode->Right)
        cutNode->Right->Parent = cutNode;

    /// change node right to newNode
    node->Right = newNode;
    newNode->Parent = node;

    /// change newNode right to cut node
    newNode->Right = cutNode;
    cutNode->Parent = newNode;

    /// update the tree
    Update(cutNode);
    Size += 2;
}

/** Point mutation into index in the tree
 * \param mutation new mutation to insert
 * \param index index to insert the mutation after 
 **/
template < typename T >
void SegmentTree::Point(size_t index, T mutation)
{
    /// create new node from mutation
    SegmentNode* newNode = CreateNode(mutation);

    /// cut up the node at the index
    auto found = Find(index);
    auto node = found.first;
    auto startIndex = found.second;

    SegmentNode* cutNode = node->Cut(index-startIndex);
    cutNode->TruncateLeft();    // remove value at index

    /// adopt nodes right
    cutNode->Right = node->Right;
    if (cutNode->Right)
        cutNode->Right->Parent = cutNode;

    /// change node right to newNode
    node->Right = newNode;
    newNode->Parent = node;

    /// change newNode right to cut node
    newNode->Right = cutNode;
    cutNode->Parent = newNode;

    /// update the tree
    Update(cutNode);
    Size += 2;
}


/*********************************************************************************
 * 
 * 
 *  Misc printing and helper destructor functions
 * 
 * 
 ********************************************************************************/


/** prints the tree breadth first **/
void SegmentTree::Print()
{
    std::cout << std::endl;
    std::queue<SegmentNode*> q;
    q.push(Root);

    while(q.size() > 0)
    {
        auto root = q.front();
        q.pop();

        root->Print();

        if (root->Left)
        {
            q.push(root->Left);
        }
        if (root->Right)
        {
            q.push(root->Right);
        }
    }
    std::cout << std::endl;

}

/** deletes root and children
 * \param root to start deleting at **/
void SegmentTree::DeleteTree(SegmentNode* root)
{
    if (root)
    {
        DeleteTree(root->Right);
        DeleteTree(root->Left); 
    }
    delete root;
}


/*********************************************************************************
 * 
 * 
 *  Delcarations of templated functions
 * 
 * 
 ********************************************************************************/
template SegmentNode* SegmentTree::CreateNode(Byte data);
template SegmentNode* SegmentTree::CreateNode(uint16_t data);

template void SegmentTree::Insert(size_t index, Byte mutation);
template void SegmentTree::Insert(size_t index, uint16_t mutation);

template void SegmentTree::Point(size_t index, Byte mutation);
template void SegmentTree::Point(size_t index, uint16_t mutation);


