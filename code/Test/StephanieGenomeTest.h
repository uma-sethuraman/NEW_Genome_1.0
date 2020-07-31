#pragma once
#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include <bitset>
#include <cstddef>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include "AbstractGenome.h"
#include "StephanieGenome.h"
#include "random.h"

void StephanieGenomeTest_One(AbstractGenome* genome) {
	std::cout << "---- test one ----" << std::endl;

	genome->resize(10);
	auto readHead = genome->data(0, 10);
	for (size_t i(0); i < genome->size(); ++i)
		readHead[i] = (std::byte)(Random::getInt(99));
	for (size_t i(0); i < genome->size(); ++i)
		std::cout << (int)GN::genomeRead<std::byte>(genome, i) << " ";
	std::cout << std::endl << std::endl;

	genome->overwrite(2, std::vector < std::byte>{44});
	genome->remove(4, 2);
	genome->insert(2, std::vector<std::byte>{ (std::byte)66 });
	genome->show();
}

void StephanieGenomeTest_Two(AbstractGenome* genome) {
	std::cout << "---- test two ----" << std::endl;
	genome->resize(10);
	auto readHead = genome->data();
	for (size_t i(0); i < genome->size(); ++i)
		readHead[i] = (std::byte)(Random::getInt(99));
	for (size_t i(0); i < genome->size(); ++i)
		std::cout << (int)GN::genomeRead<std::byte>(genome, i) << " ";
	std::cout << std::endl << std::endl;
	genome->remove(3, 4);
	genome->insert(4, std::vector<std::byte>{ (std::byte)44, (std::byte)55, (std::byte)66 });
	genome->overwrite(5, std::vector < std::byte>{88});
	genome->overwrite(4, std::vector < std::byte>{99});
	genome->show();
}

void StephanieGenomeTest_Three(AbstractGenome* genome) {
	std::cout << "---- test three ----" << std::endl;
	genome->resize(10);
	auto readHead = genome->data();
	for (size_t i(0); i < genome->size(); ++i)
		readHead[i] = (std::byte)(Random::getInt(99));
	for (size_t i(0); i < genome->size(); ++i)
		std::cout << (int)GN::genomeRead<std::byte>(genome, i) << " ";
	std::cout << std::endl << std::endl;
	genome->remove(7, 4);
	genome->insert(0, std::vector<std::byte>{ (std::byte)44, (std::byte)55 });
	genome->remove(2, 1);
	genome->overwrite(0, std::vector < std::byte>{66});
	genome->show();
}

void StephanieGenomeTest_Four(AbstractGenome* genome) {
	std::cout << "---- test four ----" << std::endl;
	genome->resize(10);
	auto readHead = genome->data();
	for (size_t i(0); i < genome->size(); ++i)
		readHead[i] = (std::byte)(Random::getInt(99));
	for (size_t i(0); i < genome->size(); ++i)
		std::cout << (int)GN::genomeRead<std::byte>(genome, i) << " ";
	std::cout << std::endl << std::endl;
	genome->remove(3, 2);
	genome->remove(5, 2);
	genome->remove(1, 4);
	genome->show();
}

void StephanieGenomeTest_Five(AbstractGenome* genome) {
	std::cout << "---- test five ----" << std::endl;
	genome->resize(10);
	auto readHead = genome->data();
	for (size_t i(0); i < genome->size(); ++i)
		readHead[i] = (std::byte)(Random::getInt(99));
	for (size_t i(0); i < genome->size(); ++i)
		std::cout << (int)GN::genomeRead<std::byte>(genome, i) << " ";
	std::cout << std::endl << std::endl;
	genome->insert(5, std::vector<std::byte>{ (std::byte)44, (std::byte)55, (std::byte)66, (std::byte)77 });
	genome->remove(2, 5);
	genome->show();
}

void StephanieGenomeTest_Six(AbstractGenome* genome) {
	std::cout << "---- test six ----" << std::endl;
	genome->resize(10);
	auto readHead = genome->data();
	for (size_t i(0); i < genome->size(); ++i)
		readHead[i] = (std::byte)(Random::getInt(99));
	for (size_t i(0); i < genome->size(); ++i)
		std::cout << (int)GN::genomeRead<std::byte>(genome, i) << " ";
	std::cout << std::endl << std::endl;
	genome->insert(5, std::vector<std::byte>{ (std::byte)11, (std::byte)22, (std::byte)33 });
	genome->remove(1, 5);
	genome->insert(4, std::vector<std::byte>{ (std::byte)44, (std::byte)55, (std::byte)66, (std::byte)77 });
	genome->insert(6, std::vector<std::byte>{ (std::byte)13, (std::byte)23, (std::byte)33, (std::byte)43 });
	genome->remove(5, 4);
	genome->insert(1, std::vector<std::byte>{ (std::byte)42, (std::byte)52, (std::byte)62 });
	genome->remove(4, 7);
	genome->insert(6, std::vector<std::byte>{ (std::byte)79 });
	genome->show();
}

void StephanieGenomeTest_Seven(AbstractGenome* genome) {
	std::cout << "---- test seven ----" << std::endl;
	genome->resize(10);
	auto readHead = genome->data();
	for (size_t i(0); i < genome->size(); ++i)
		readHead[i] = (std::byte)(Random::getInt(99));
	for (size_t i(0); i < genome->size(); ++i)
		std::cout << (int)GN::genomeRead<std::byte>(genome, i) << " ";
	std::cout << std::endl;
	std::vector<std::byte> myVector = { (std::byte)1, (std::byte)2, (std::byte)3 };
	genome->insert(0, myVector);
	myVector = { (std::byte)1, (std::byte)2, (std::byte)3 };
	genome->insert(0, myVector);
	myVector = { (std::byte)4, (std::byte)4, (std::byte)4 };
	genome->overwrite(2, myVector);
	genome->remove(1, 2);
	genome->show();
}

void StephanieGenomeTest_Eight(AbstractGenome* genome) {
	std::cout << "---- test eight ----" << std::endl;
	genome->resize(10);
	auto readHead = genome->data();
	for (size_t i(0); i < genome->size(); ++i)
		readHead[i] = (std::byte)(Random::getInt(99));
	for (size_t i(0); i < genome->size(); ++i)
		std::cout << (int)GN::genomeRead<std::byte>(genome, i) << " ";
	std::cout << std::endl;
	std::vector<std::byte> myVector = { (std::byte)11, (std::byte)22, (std::byte)33 };
	genome->insert(9, myVector);
	genome->show();
	myVector = { (std::byte)44, (std::byte)55 };
	genome->insert(12, myVector);
	genome->show();
	myVector = { (std::byte)66 };
	genome->overwrite(1, myVector );
	genome->show();
	genome->remove(9, 3);
	genome->show();
}

void StephanieGenomeTest_Nine(AbstractGenome* genome) {
	std::cout << "---- test nine ----" << std::endl;
	auto readHead = genome->data();
	for (size_t i(0); i < genome->size(); ++i)
		readHead[i] = (std::byte)(Random::getInt(99));
	for (size_t i(0); i < genome->size(); ++i)
		std::cout << (int)GN::genomeRead<std::byte>(genome, i) << " ";
	std::cout << std::endl;
	std::vector<std::byte> myVector = { 
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33,
		(std::byte)11, (std::byte)22, (std::byte)33
	};
	genome->insert(9, myVector);
	genome->insert(99, myVector);
	genome->insert(50, myVector);
	genome->insert(63, myVector);
	genome->insert(11, myVector);
}
