/**
 * \file HandlerTest.h
 * \author Victoria Cao
 * 
 * \brief namespace for testing handler functions
 **/

#include <cassert>
#include <memory>

#include "../AbstractGenome.h"
#include "../Genome.h"
#include "../GenomeLite.h"
#include "../Handler.h"
#include "../Mutation.h"

typedef char Byte; ///< Byte for easy viewing

/** handler testing functions **/
namespace TestHandler
{
    void testPointMutation()
    {
         std::cout << "\nTest Point Mutation" << std::endl;
        /// creating genome
        std::shared_ptr<AbstractGenome> genome = std::make_shared<Genome>(8);

        ///init genome
        for (size_t i = 0; i < genome->size(); i++)
        {
            genome->data()[i] = i;
        }

        std::shared_ptr<GenomeLite> genomeMutation = std::make_shared<GenomeLite>(genome);
        Handler handler(genomeMutation);

        /// testing deleting 0
        handler.reset();
        std::cout << "Test Point Front: ";
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 0)
            {
                handler.PointMutation((Byte)1);
            }	
        }

        int countOnes = 0;
        handler.reset();
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 1)
                countOnes++;
            handler.next();
        }
        assert(genomeMutation->size() == 7);
        assert(genomeMutation->segmentsCount() == 2);
        assert(countOnes == 2);

        std::cout << "Passed" << std::endl;

        /// testing deleting 7
        countOnes = 0;
        handler.reset();
        std::cout << "Test Point Back: ";
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 7)
            {
                handler.PointMutation((Byte)1);
            }	
        }

        countOnes = 0;
        handler.reset();
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 1)
                countOnes++;
            handler.next();
        }
        assert(genomeMutation->size() == 7);
        assert(genomeMutation->segmentsCount() == 3);
        assert(countOnes == 3);


        std::cout << "Passed" << std::endl;

        /// testing deleting 5
        handler.reset();
        std::cout << "Test Point Middle: ";
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 0)
            {
                handler.PointMutation((Byte)1);
            }	
        }

        countOnes = 0;
        handler.reset();
        while (handler.index() < genomeMutation->size())
        {
            if (static_cast<int>(*handler) == 5)
                countOnes++;
            handler.next();
        }
        assert(genomeMutation->size() == 7);
        assert(genomeMutation->segmentsCount() == 5);
        assert(countOnes == 4);


        std::cout << "Passed" << std::endl;


        std::cout << "Point Mutation Passed" << std::endl; 

    }

    void testInsertionMutation()
    {

    }

    void testCopyMutation()
    {
        

    }

    void testDeleteMutation()
    {
        std::cout << "\nTest Delete Mutation" << std::endl;
        /// creating genome
        std::shared_ptr<AbstractGenome> genome = std::make_shared<Genome>(8);

        ///init genome
        for (size_t i = 0; i < genome->size(); i++)
        {
            genome->data()[i] = i;
        }

        std::shared_ptr<GenomeLite> genomeMutation = std::make_shared<GenomeLite>(genome);
        Handler handler(genomeMutation);

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
        assert(genomeMutation->size() == 7);
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
        assert(genomeMutation->size() == 6);
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
        assert(genomeMutation->size() == 5);
        assert(genomeMutation->segmentsCount() == 2);

        std::cout << "Passed" << std::endl;


        std::cout << "Delete Mutation Passed" << std::endl;
    }
}