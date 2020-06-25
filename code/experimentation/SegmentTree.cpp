/**
 * \file SegmentTree.cpp
 * \author Victoria Cao
 **/

#include "SegmentTree.h"

int SegmentTree::GetHeight(SegmentNode* node)
{
    if (node)
        return node->Height;
    else
        return -1;
    
}

int SegmentTree::GetBalance(SegmentNode* node)
{
    auto left = GetHeight(node->Left);
    auto right = GetHeight(node->Right);

    return left - right;
}

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


        node->Weight = node->Size + right + left;
    }
}

void SegmentTree::UpdateHeight(SegmentNode* node)
{
    if (node)
    {   
        int left = GetHeight(node->Left);
        int right = GetHeight(node->Right);
        
        node->Height = std::max(left, right) +1;
    }
}

SegmentNode* SegmentTree::ReBalance(SegmentNode* node)
{
    auto balance = GetBalance(node);

    if (balance == 2)
    {
        if (GetBalance(node->Left) == -1)
            RotateLeft(node->Left);
        return RotateRight(node);
    }
    else if (balance == -2)
    {
        if (GetBalance(node->Right) == 1)
            RotateRight(node->Right);
        return RotateLeft(node);
    }

    return node;
}

void SegmentTree::Update(SegmentNode* root)
{
    SegmentNode* node = root;
    while (node)
    {
        UpdateWeight(node);
        UpdateHeight(node);
        node = ReBalance(node)->Parent;
    }

}

void SegmentTree::Delete(size_t index)
{
    auto found = Find(index);
    auto node = found.first;
    auto startIndex = found.second;
    
    // print();
    std::cout << node << " " << index-startIndex << std::endl;

    SegmentNode* newNode = node->Cut(index-startIndex);
    newNode->TruncateLeft();

    /// adopt nodes right
    newNode->Right = node->Right;
    if (newNode->Right)
        newNode->Right->Parent = newNode;

    /// change node right to cut node
    node->Right = newNode;
    newNode->Parent = node;

    Update(newNode);
}

void SegmentTree::Insert(SegmentNode* root, size_t index)
{}


/** prints the tree breadth first **/
void SegmentTree::print()
{
    std::cout << std::endl;
    std::queue<SegmentNode*> q;
    q.push(Root);

    while(q.size() > 0)
    {
        auto root = q.front();
        q.pop();

        root->print();

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

