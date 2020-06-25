/** 
 * \file TestTree.h
 * \author Victoria Cao
 * 
 * \brief Testing rope functions
 **/

#pragma once
#include <memory>

#include "../GeneSegment.h"
#include "../SegmentTree.h"

namespace TestTree
{
    void TestDelete()
    {
        size_t size = 100;
        Byte* data = new Byte[size];
        for (size_t i(0); i < size; i++) { data[i] = (Byte)i; }

        SegmentNode* root = new SegmentNode(std::make_shared< GeneSegment >(data, size, true));
        SegmentTree* tree = new SegmentTree(root);

        // tree->print();

        tree->Delete(10);
        tree->Delete(20);
        tree->Delete(30);
        tree->Delete(40);
        tree->Delete(50);

        tree->print();


        delete tree;
    }
    
};