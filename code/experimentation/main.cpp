/** 
 * \file main.cpp
 * \author Victoria Cao
 **/

#include <iostream>
#include "testing/TestTree.h"

typedef char Byte; // c++17 Byte doesn't always work


/** main testing **/
int main() {
	/// Handler Testing
	TestTree::TestDelete();
	TestTree::TestInsert();
	TestTree::TestPoint();

	return(0);
}
