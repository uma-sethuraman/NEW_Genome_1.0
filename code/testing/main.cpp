/** 
 * \file main.cpp
 * \author Victoria Cao
 **/

#include <iostream>
#include <string>

#include "../experimentation/GenomeLite/testing/TestHandler.h"
#include "../experimentation/GenomeLite/testing/TestHandlerMutations.h"
#include "../experimentation/GenomeLite/testing/TestGenomeLite.h"

typedef char Byte; // c++17 Byte doesn't always work


#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


void Title(std::string title)
{
	std::cout << BOLDRED << "\n" << title << RESET << std::endl;
}

void EndTest()
{
	std::cout << std::endl;
}

/** main testing **/
int main() {
	// /// Tree Testing.
	// TestTree::TestDelete();
	// TestTree::TestInsert();
	// TestTree::TestPoint();

	/// Handler Testing
	// Title("HANDLER TESTING");

	// TestHandler::TestConstructor();
	// TestHandler::TestIterator();
	// TestHandler::TestMoveTo();

	// EndTest();

	// // Handler Mutation Testing
	// Title("HANDLER MUTATION TESTING");

	// TestHandlerMutations::TestDeletion();
	// TestHandlerMutations::TestInsertion();
	// TestHandlerMutations::TestPoint();

	// EndTest();

	// // GenomeLite Testing
	// Title("GENOMELITE TESTING");

	// TestGenomeLite::TestCopyConstructor();
	TestGenomeLite::TestReallocation();

	// EndTest();


	return(0);
}