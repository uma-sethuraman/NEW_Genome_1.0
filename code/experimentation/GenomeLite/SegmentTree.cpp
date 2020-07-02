/**
 * \file SegmentTree.cpp
 * \author Victoria Cao
 **/


#include <cstdint>
#include <vector>

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
        return node->GetHeight();
    else
        return -1;
    
}

/** Calculates the balance factor of node
 * \param node to get the balance factor of 
 * \return balance factor of the node
 **/
int SegmentTree::GetBalance(SegmentNode* node)
{
    auto left = GetHeight(node->GetLeft(Pool));
    auto right = GetHeight(node->GetRight(Pool));

    return left - right;
}

/** Updates height of Node
 * \param node to update height of
 **/
void SegmentTree::UpdateHeight(SegmentNode* node)
{
    if (node)
    {   
        // node height is one plus the taller subtree
        node->SetHeight(std::max(GetHeight(node->GetLeft(Pool)), GetHeight(node->GetRight(Pool))) +1);
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
        if (node->GetRight(Pool))
            right = node->GetRight(Pool)->GetWeight();
        if (node->GetLeft(Pool))
            left = node->GetLeft(Pool)->GetWeight();

        // node weight is sum of left and right subtrees and it's own weight
        node->SetWeight(node->GetSize() + right + left);
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
        node = ReBalance(node)->GetParent(Pool);
    }
}

/** Creates a new node in the pool 
 * \param data creates new segment in tree
 * \param size of data
 * \return pointer to new node
 **/
SegmentNode* SegmentTree::CreateNode(Byte* data, size_t size)
{
    return Pool->CreateNode(std::make_shared< GeneSegment >(data, size, true));
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
    SegmentNode* newRoot = root->GetLeft(Pool); // new newRoot 
    SegmentNode* rootAdopt = newRoot->GetRight(Pool);

    if (root->GetParent(Pool))   /// change root parent
    {
        if (root->GetParent(Pool)->GetLeft(Pool) == root)
        {
            newRoot->SetParent(root->GetParent(Pool));
            newRoot->GetParent(Pool)->SetLeft(newRoot);
        }
        else
        {
            newRoot->SetParent(root->GetParent(Pool));
            newRoot->GetParent(Pool)->SetRight(newRoot);
        }
    }
    else    /// changing newRoot 
    {
        newRoot->SetParent(nullptr);
        Root = newRoot;
    }

    // change root and new root
    root->SetParent(newRoot);
    newRoot->SetRight(root);

    // change root and root adopt
    root->SetLeft(rootAdopt);
    if (rootAdopt)
        rootAdopt->SetParent(root);

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
    SegmentNode* newRoot = root->GetRight(Pool); // new newRoot 
    SegmentNode* rootAdopt = newRoot->GetLeft(Pool);

    if (root->GetParent(Pool))   /// change root parent
    {
        if (root->GetParent(Pool)->GetLeft(Pool) == root)
        {
            newRoot->SetParent(root->GetParent(Pool));
            newRoot->GetParent(Pool)->SetLeft(newRoot);
        }
        else
        {
            newRoot->SetParent(root->GetParent(Pool));
            newRoot->GetParent(Pool)->SetRight(newRoot);
        }
    }
    else    /// changing newRoot 
    {
        newRoot->SetParent(nullptr);
        Root = newRoot;
    }

    // change root and new root
    root->SetParent(newRoot);
    newRoot->SetLeft(root);

    // change root and root adopt
    root->SetRight(rootAdopt);
    if (rootAdopt)
        rootAdopt->SetParent(root);

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
        if (GetBalance(node->GetLeft(Pool)) == -1) // Right Left case
            RotateLeft(node->GetLeft(Pool));
        return RotateRight(node);
    }
    else if (balance == -2) // Left case
    {
        if (GetBalance(node->GetRight(Pool)) == 1) // Left Right case
            RotateRight(node->GetRight(Pool));
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
    size_t right = node->GetWeight();    // Right boundary of node
    if (node->GetLeft(Pool))
        left += node->GetLeft(Pool)->GetWeight();
    if (node->GetRight(Pool))
        right -= node->GetRight(Pool)->GetWeight();

    size_t min = 0;     // Left boundary of node's left subtree
    size_t max = node->GetWeight();  // Right boundary of node's right subtree
    
    /// Find the segment that index lies within
    while(index < left || index >= right)
    {
        if (index >= right)
        { 
            min = right;
            node = node->GetRight(Pool);
        }  
        else
        {
            max = left;
            node = node->GetLeft(Pool);
        }
        
        // Update node's segment boundary
        left = min;
        right = max;
        if (node->GetLeft(Pool))
            left += node->GetLeft(Pool)->GetWeight();
        if (node->GetRight(Pool))
            right -= node->GetRight(Pool)->GetWeight();
           
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
    auto offset = index-found.second;

    if (offset && offset != node->GetSize()-1)    // Middle case
    {
        SegmentNode* cutNode = node->Cut(Pool, offset);
        cutNode->TruncateLeft();    // remove value at index

        /// adopt nodes right
        cutNode->SetRight(node->GetRight(Pool));
        if (cutNode->GetRight(Pool))
            cutNode->GetRight(Pool)->SetParent(cutNode);

        /// change node right to cut node
        node->SetRight(cutNode);
        cutNode->SetParent(node);

        /// update the tree
        ++Size;
        Update(cutNode);
    }
    else if (!offset) // Front case
    {
        node->TruncateLeft();
        Update(node);
    }
    else // Back case
    {
        node->TruncateRight();
        Update(node);
    }

}

/** Point mutation into index in the tree
 * \param index index to insert the mutation after 
 * \param mutation new mutation to insert
 * \param size of mutation
 **/
void SegmentTree::Insert(size_t index, Byte* mutation, size_t size)
{
    /// create new node from mutation
    SegmentNode* newNode = CreateNode(mutation, size);

    /// cut up the node at the index
    auto found = Find(index);
    auto node = found.first;
    auto offset = index-found.second;

    if (offset)
    {
        SegmentNode* cutNode = node->Cut(Pool, offset);

        /// adopt nodes right
        cutNode->SetRight(node->GetRight(Pool));
        if (cutNode->GetRight(Pool))
            cutNode->GetRight(Pool)->SetParent(cutNode);

        /// change node right to newNode
        node->SetRight(newNode);
        newNode->SetParent(node);

        /// change newNode right to cut node
        newNode->SetRight(cutNode);
        cutNode->SetParent(newNode);

        /// update the tree
        Update(cutNode);
        ++Size;
    }
    else
    {
        /// adopt nodes right
        newNode->SetLeft(node->GetLeft(Pool));
        if (node->GetLeft(Pool))
            newNode->GetLeft(Pool)->SetParent(newNode);

        /// change node Left to newNode
        node->SetLeft(newNode);
        newNode->SetParent(node);

        // balance tree
        Update(newNode);
    }
  
    ++Size;
}

/** Point mutation into index in the tree
 * \param index index to insert the mutation after 
 * \param mutation new mutation to insert
 * \param size of mutation
 **/
void SegmentTree::Point(size_t index, Byte* mutation, size_t size)
{
    /// create new node from mutation
    SegmentNode* newNode = CreateNode(mutation, size);

    /// cut up the node at the index
    auto found = Find(index);
    auto node = found.first;
    auto offset = index-found.second;


    if (offset && offset != node->GetSize()-1) // middle case
    {
        SegmentNode* cutNode = node->Cut(Pool, offset);
        cutNode->TruncateLeft();    // remove value at index

        /// adopt nodes right
        cutNode->SetRight(node->GetRight(Pool));
        if (cutNode->GetRight(Pool))
            cutNode->GetRight(Pool)->SetParent(cutNode);

        /// change node right to newNode
        node->SetRight(newNode);
        newNode->SetParent(node);

        /// change newNode right to cut node
        newNode->SetRight(cutNode);
        cutNode->SetParent(newNode);

        /// update the tree
        Update(cutNode);
        Size += 1;
    }
    else if (!offset) // Front case
    {
        node->TruncateLeft();

        /// adopt nodes right
        newNode->SetLeft(node->GetLeft(Pool));
        if (node->GetLeft(Pool))
            newNode->GetLeft(Pool)->SetParent(newNode);

        /// change node right to newNode
        node->SetLeft(newNode);
        newNode->SetParent(node);

        // balance tree
        Update(newNode);
    }
    else // Back case
    {
        node->TruncateRight();

        /// adopt nodes right
        newNode->SetRight(node->GetRight(Pool));
        if (node->GetRight(Pool))
            newNode->GetRight(Pool)->SetParent(newNode);

        /// change node right to newNode
        node->SetRight(newNode);
        newNode->SetParent(node);

        // balance tree
        Update(newNode);
    }
   

    // update size of tree
    Size += 1;  
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

        if (root->GetLeft(Pool))
        {
            q.push(root->GetLeft(Pool));
        }
        if (root->GetRight(Pool))
        {
            q.push(root->GetRight(Pool));
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
        DeleteTree(root->GetRight(Pool));
        DeleteTree(root->GetLeft(Pool)); 
    }
    delete root;
}


