/** 
 * \file main.cpp
 * \author Victoria Cao
 **/

#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include <bitset>
#include <cstddef>
#include <memory>

#include "utilities.h"
#include "AbstractGenome.h"
#include "Genome.h"
#include "GenomeLite.h"

typedef char Byte; // c++17 Byte doesn't always work

/** main testing **/
int main() {
	/// creating genome
	std::shared_ptr<AbstractGenome> genome = std::make_shared<Genome>(8);

	///init genome
	for (size_t i = 0; i < genome->size(); i++)
	{
		genome->data()[i] = i;
	}

	std::cout << "main " << std::endl;
	std::cout << std::endl;

	GenomeLite genomeMutation(genome);

	auto handler = genomeMutation.begin();
	while (handler.index() < genomeMutation.size())
	{
		std::cout << handler.index() << " " << (unsigned long)*handler << std::endl;
		if (handler.index() == (size_t)5)
		{
			handler.DeleteMutation();
		}		
		handler++;
	}
	
	std::cout << "test cutting " << std::endl;
	std::cout << std::endl;

	handler = genomeMutation.begin();
	while (handler.index() < genomeMutation.size())
	{
		std::cout << handler.index() << " " << (unsigned long)*handler << std::endl;
		handler++;
	}
	

	return(0);
}
