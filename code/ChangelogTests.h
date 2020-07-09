#include "AbstractGenome.h"
#include <string>

void printResults(std::vector<std::byte> answer, AbstractGenome* gen, std::string testName);
void pointTest1(bool debug);
void deleteTest1(bool debug);
void deleteTest2(bool debug);
void deleteTest3(bool debug);
void deleteTest4(bool debug);
void deleteTest5(bool debug);
void deleteTest6(bool debug);
void deleteTest7(bool debug);
void deleteTest8(bool debug);
void deleteTest9(bool debug);
void insertTest1(bool debug);
void insertTest2(bool debug);
void insertTest3(bool debug);
void insertTest4(bool debug);
void insertTest5(bool debug);
void allMutationsTest1(bool debug);
void allMutationsTest2(bool debug);
void allMutationsTest3(bool debug);
void runPointTests(bool debug);
void runDeleteTests(bool debug);
void runInsertTests(bool debug);
void runChangelogTests(bool debug);