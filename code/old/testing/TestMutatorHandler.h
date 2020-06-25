/**
 * \file TestMutatorHandler.h
 * \author Victoria Cao
 * 
 * \brief namespace for testing handler functions
 **/

#include <cassert>
#include <memory>

#include "../AbstractGenome.h"
#include "../Genome.h"
#include "../GenomeLite.h"
#include "../GeneSegment.h"
#include "../handlers/MutatorHandler.h"

typedef char Byte; ///< Byte for easy viewing

/** handler testing functions **/
namespace TestMutatorHandler
{
    void testPointMutation()
    {
        size_t size = 8;

        std::cout << "\nTest Point Mutation" << std::endl;
        /// creating genome
        AbstractGenome* genome = new Genome(size);

        ///init genome
        for (size_t i = 0; i < genome->size(); i++)
        {
            genome->data()[i] = i;
        }

        GenomeLite* genomeMutation = new GenomeLite(genome);
        MutatorHandler handler(genomeMutation);

        // genomeMutation->print();

        /// testing point 0
        handler.reset();
        int countOnes = 0;
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 0)
            {
                auto mutation = std::make_shared< GeneSegment >((Byte)1);
                handler.PointMutation(mutation);
            }	
            handler.next();
        }

        countOnes = 0;
        handler.reset();
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 1)
                countOnes++;
            handler.next();
        }
        
        assert(genomeMutation->size() == size);
        assert(genomeMutation->segmentsCount() == 2);
        assert(countOnes == 2);

        std::cout << "Test Point Front: Passed" << std::endl;

        /// testing point 7
        countOnes = 0;
        handler.reset();
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 7)
            {
                auto mutation = std::make_shared< GeneSegment >((Byte)1);
                handler.PointMutation(mutation);
            }	
            handler.next();
        }
  

        countOnes = 0;
        handler.reset();
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 1)
                countOnes++;
            handler.next();
        }
        assert(genomeMutation->size() == size);
        assert(genomeMutation->segmentsCount() == 3);
        assert(countOnes == 3);


        std::cout << "Test Point Back: Passed" << std::endl;

        /// testing point 5
        handler.reset();
        while (handler.index() < genomeMutation->size())
        {
            // handler.print();
            if (static_cast<int>(*handler) == 5)
            {
                auto mutation = std::make_shared< GeneSegment >((Byte)1);
                handler.PointMutation(mutation);
            }	
            handler.next();
        }

        countOnes = 0;
        handler.reset();
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 1)
                countOnes++;
            handler.next();
        }

        // genomeMutation->print(); 
        assert(genomeMutation->size() == size);
        assert(genomeMutation->segmentsCount() == 5);
        assert(countOnes == 4);

        std::cout << "Test Point Middle: Passed" << std::endl;


        // genomeMutation->print();



        delete genomeMutation;
        delete genome;

    }

    void testInsertionMutation()
    {
        size_t size = 8;

        std::cout << "\nTest Insert Mutation" << std::endl;
        /// creating genome
        AbstractGenome* genome = new Genome(size);

        ///init genome
        for (size_t i = 0; i < genome->size(); i++)
        {
            genome->data()[i] = i;
        }

        GenomeLite* genomeMutation = new GenomeLite(genome);
        MutatorHandler handler(genomeMutation);

        /// testing inserting 0
        handler.reset();
        int countOnes = 0;
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 0)
            {
                auto mutation = std::make_shared< GeneSegment >((Byte)1);
                handler.InsertMutation(mutation);
            }	
            handler.next();
        }

        countOnes = 0;
        handler.reset();
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 1)
                countOnes++;
            handler.next();
        }
        assert(genomeMutation->size() == size+1);
        assert(genomeMutation->segmentsCount() == 3);
        assert(countOnes == 2);

        std::cout << "Test Insert Front: Passed" << std::endl;

        /// testing inserting 7
        countOnes = 0;
        handler.reset();
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 7)
            {
                auto mutation = std::make_shared< GeneSegment >((Byte)1);
                handler.InsertMutation(mutation);
            }	
            handler.next();
        }

        countOnes = 0;
        handler.reset();
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 1)
                countOnes++;
            handler.next();
        }
        assert(genomeMutation->size() == size+2);
        assert(genomeMutation->segmentsCount() == 4);
        assert(countOnes == 3);


        std::cout << "Test Insert Back: Passed" << std::endl;

        /// testing deleting 5
        handler.reset();
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 5)
            {
                auto mutation = std::make_shared< GeneSegment >((Byte)1);
                handler.InsertMutation(mutation);
            }	
            handler.next();
        }

        countOnes = 0;
        handler.reset();
        while (handler.index() < genomeMutation->size())
        {
            // std::cout << handler.index() << ": " << (int)*handler << std::endl;
            if (static_cast<int>(*handler) == 1)
                countOnes++;
            handler.next();
        }
        assert(genomeMutation->size() == size+3);
        assert(genomeMutation->segmentsCount() == 6);
        assert(countOnes == 4);

        std::cout << "Test Insert Middle: Passed" << std::endl;

        std::cout << "Insertion Mutation Passed" << std::endl; 

        delete genomeMutation;
        delete genome;
    }


    void testDeleteMutation()
    {
        size_t size = 8;

        std::cout << "\nTest Delete Mutation" << std::endl;
        /// creating genome
        AbstractGenome* genome = new Genome(size);

        ///init genome
        for (size_t i = 0; i < genome->size(); i++)
        {
            genome->data()[i] = i;
        }

        GenomeLite* genomeMutation = new GenomeLite(genome);
        MutatorHandler handler(genomeMutation);

        /// testing deleting 0
        handler.reset();
        std::cout << "Test Delete Front: ";
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 0)
            {
                handler.DeleteMutation();
            }	
            assert((int)(*handler) != 0);

            handler.next();
        }
        assert(genomeMutation->size() == size-1);
        assert(genomeMutation->segmentsCount() == 1);

        std::cout << "Passed" << std::endl;

        /// testing deleting 7
        handler.reset();
        std::cout << "Test Delete Back: ";
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 7)
            {
                handler.DeleteMutation();
            }	
            else
            {
                assert((int)(*handler) != 7);
                handler.next();
            }
        }
        assert(genomeMutation->size() == size-2);
        assert(genomeMutation->segmentsCount() == 1);

        std::cout << "Passed" << std::endl;

        /// testing deleting 5
        handler.reset();
        std::cout << "Test Delete Middle: ";
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 5)
            {
                handler.DeleteMutation();
            }	
            assert((int)(*handler) != 5);

            handler.next();
        }
        assert(genomeMutation->size() == size-3);
        assert(genomeMutation->segmentsCount() == 2);

        std::cout << "Passed" << std::endl;


        std::cout << "Delete Mutation Passed" << std::endl;

        delete genomeMutation;
        delete genome;
    }

    void testCopyMutation()
    {
        std::cout << "WARNING: Copy mutation test not configured" << std::endl;    

    }
}