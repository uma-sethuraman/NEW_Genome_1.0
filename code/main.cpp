/** 
 * \file main.cpp
 * \author Victoria Cao
 **/

#include <iostream>

#include "testing/TestMutatorHandler.h"
#include "testing/TestHandler.h"

typedef char Byte; // c++17 Byte doesn't always work


/** main testing **/
int main() {
	/// Handler Testing
	TestMutatorHandler::testPointMutation();
	TestMutatorHandler::testInsertionMutation();
	TestMutatorHandler::testDeleteMutation();
	// TestMutatorHandler::testCopyMutation();

	TestHandler::testSearch();

	return(0);
}
