/** 
 * \file TestTree.h
 * \author Victoria Cao
 * 
 * \brief Testing segment tree functions
 **/

#pragma once
#include <memory>
#include <cassert>

#include "../GeneSegment.h"
#include "../SegmentTree.h"

namespace TestTree
{
    void TestDelete()
    {
        // initializing tree 
        size_t size = 100;
        Byte* data = new Byte[size];
        for (size_t i(0); i < size; i++) { data[i] = (Byte)i; }

        SegmentNode* root = new SegmentNode(std::make_shared< GeneSegment >(data, size, true));
        SegmentTree* tree = new SegmentTree(root);

        // cut up into 6 chunks
        tree->Delete(10);
        tree->Delete(20);
        tree->Delete(30);
        tree->Delete(40);
        tree->Delete(50);

        // tests height, site count, and size of tree
        assert(tree->GetRoot()->GetHeight() <= 3);
        assert(tree->GetSiteCount() == 95);
        assert(tree->GetSize() == 6);

        delete tree;

        std::cout << "Test Delete: Passed" << std::endl;
    }

    void TestInsert()
    {
        // initializing tree 
        size_t size = 100;
        Byte* data = new Byte[size];
        for (size_t i(0); i < size; i++) { data[i] = (Byte)i; }

        SegmentNode* root = new SegmentNode(std::make_shared< GeneSegment >(data, size, true));
        SegmentTree* tree = new SegmentTree(root);

        // insert 3 mutations
        SegmentNode* mut1 = new SegmentNode(std::make_shared< GeneSegment >((Byte)1));
        tree->Insert(10, mut1);
        SegmentNode* mut2 = new SegmentNode(std::make_shared< GeneSegment >((Byte)2));
        tree->Insert(20, mut2);
        SegmentNode* mut3 = new SegmentNode(std::make_shared< GeneSegment >((Byte)3));
        tree->Insert(30, mut3);

        // tests height, site count, and size of tree
        assert(tree->GetRoot()->GetHeight() <= 3);
        assert(tree->GetSiteCount() == 103);
        assert(tree->GetSize() == 7);

        delete tree;

        std::cout << "Test Insert: Passed" << std::endl;
    }

    void TestPoint()
    {
        // initializing tree 
        size_t size = 100;
        Byte* data = new Byte[size];
        for (size_t i(0); i < size; i++) { data[i] = (Byte)i; }

        SegmentNode* root = new SegmentNode(std::make_shared< GeneSegment >(data, size, true));
        SegmentTree* tree = new SegmentTree(root);

        // insert 3 mutations
        SegmentNode* mut1 = new SegmentNode(std::make_shared< GeneSegment >((Byte)1));
        tree->Point(10, mut1);
        SegmentNode* mut2 = new SegmentNode(std::make_shared< GeneSegment >((Byte)2));
        tree->Point(20, mut2);
        SegmentNode* mut3 = new SegmentNode(std::make_shared< GeneSegment >((Byte)3));
        tree->Point(30, mut3);

        // tests height, site count, and size of tree
        assert(tree->GetRoot()->GetHeight() <= 3);
        assert(tree->GetSiteCount() == 100);
        assert(tree->GetSize() == 7);

        delete tree;

        std::cout << "Test Point: Passed" << std::endl;
    }
    
    
};