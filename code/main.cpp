/** 
 * \file main.cpp
 * \author Victoria Cao
 **/

#include <iostream>

#include "testing/HandlerTest.h"

typedef char Byte; // c++17 Byte doesn't always work


/** main testing **/
int main() {
	/// Handler Testing
	TestHandler::testPointMutation();
	TestHandler::testInsertionMutation();
	TestHandler::testCopyMutation();
	TestHandler::testDeleteMutation();


	return(0);
}
