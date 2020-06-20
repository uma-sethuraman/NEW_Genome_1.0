/** 
 * \file main.cpp
 * \author Victoria Cao
 **/

#include <iostream>

#include "testing/TestMutatorHandler.h"

typedef char Byte; // c++17 Byte doesn't always work


/** main testing **/
int main() {
	/// Handler Testing
	TestMutatorHandler::testPointMutation();
	TestMutatorHandler::testInsertionMutation();
	// TestMutatorHandler::testCopyMutation();
	TestMutatorHandler::testDeleteMutation();


	return(0);
}
