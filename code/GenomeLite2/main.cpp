/** 
 * \file main.cpp
 * \author Victoria Cao
 **/

#include <iostream>
#include <string>

#include "GenomeLite.h"

typedef std::byte Byte; // c++17 Byte doesn't always work


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

/** main testing **/
int main() {
	GenomeLite tree;

    SegmentNode* root = new SegmentNode();

    tree.Root = root;

    tree.Insert(0, Key(5, std::make_shared< GeneSegment >(5)));

    delete root;

	return(0);
}