#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include <bitset>
#include "AbstractGenome.h"
#include "TestGenome.h"
#include <cstddef>

void runTests(AbstractGenome* genome) {
	//initialize genome to 8 sites
	genome->resize(8);
	auto readHead = genome->data();
	for (size_t i(0); i < genome->size(); ++i)
		readHead[i] = (std::byte)(i + 1);
	readHead[7] = (std::byte)(0b10000001); // 129, or -127

	//GN::genomeWrite(genome, 5, uint8_t(10));


	/* this test demonstrates no time is spent copying into num if we use references.
	   * Note: genomeRead<>() is defined in AbstractGenome.h
	   */
	{
		std::byte& num = GN::genomeRead<std::byte>(genome, 0);
		assert((int)num == 1); // same as above
		genome->data()[0] = (std::byte)3;
		assert((int)num == 3);
		num = (std::byte)1; // reset it to 1
	}

	int num;

	// 1-byte read
	num = (int)GN::genomeRead<std::byte>(genome, 0);
	assert(num == 0b00000001);
	assert(num == 1); // same as above

	// 1-byte read (with named parameters for readability)
	num = (int)GN::genomeRead<std::byte>({ .genome = genome, .index = 0 });
	assert(num == 0b00000001);
	assert(num == 1); // same as above

	// 2-byte read
	num = GN::genomeRead<uint16_t>(genome, 0);
	assert(num == 0b00000010'00000001);
	assert(num == 513); // same as above

	// 4-byte read
	num = GN::genomeRead<uint32_t>(genome, 0);
	assert(num == 0b00000100'00000011'00000010'00000001);
	assert(num == 67'305'985); // same as above

	// 1-double read
	double& dbl = GN::genomeRead<double>(genome, 0);
	uint64_t bitanswer = 0b10000001'00000111'00000110'00000101'00000100'00000011'00000010'00000001; // roughly, = -1.04917e-303
	assert(dbl == reinterpret_cast<double*>(&bitanswer)[0]);

	// 2's complement
	// left-most bit interpretation:
	// unsigned vs signed
	num = GN::genomeRead<uint8_t>(genome, 7); // range [0,255] [00000000, 11111111]
	assert(num == 0b10000001);
	assert(num == 129); // same as above
	num = GN::genomeRead<int8_t>(genome, 7); // range [-128,127] [10000000,01111111]
	assert(num == static_cast<int8_t>(0b10000001));
	assert(num == -127); // same as above

	// byte-array read with no copy
	{
		std::array<uint8_t, 4>& arr = GN::genomeRead<std::array<uint8_t, 4>>(genome, 0);
		for (size_t i(0); i < arr.size(); i++)
			assert(arr[i] == i + 1);
	}

	// int32-array read with no copy
	{
		std::array<unsigned int, 2>& arr = GN::genomeRead<std::array<unsigned int, 2>>(genome, 0);
		assert(arr[0] == 67'305'985);
		assert(arr[1] == 2'164'721'157);
	}

	std::cout << "all read tests passed" << std::endl;

	// write single byte
	GN::genomeWrite<uint8_t>(genome, 0, 255);
	assert(GN::genomeRead<uint8_t>(genome, 0) == 255);

	// write several bytes
	for (int i = 0; i < 4; i++) { // rewrite first 4 bytes with (2,4,6,8)
		GN::genomeWrite<uint8_t>({ .genome = genome, .index = i, .value = uint8_t((i + 1) * 2) });
		assert(GN::genomeRead<uint8_t>(genome, i) == (i + 1) * 2);
	}

	// write array of integers (and let compiler guess the type!)
	{
		std::array<uint32_t, 2> arr = { 42,213 };
		GN::genomeWrite(genome, 0, arr);
		auto& result = GN::genomeRead<std::array<uint32_t, 2>>(genome, 0);
		assert(result[0] == 42);
		assert(result[1] == 213);
	}

	std::cout << "all write tests passed" << std::endl;
}

void runGeneTest(AbstractGenome* genome) {

	size_t index = 0;
	GN::genomeWrite(genome, index, int(10));
	index += 4;
	GN::genomeWrite(genome, index, int(15));
	index += 4;
	GN::genomeWrite(genome, index, std::byte(30));
	index++;
	GN::genomeWrite(genome, index, std::byte(31));
	index++;
	GN::genomeWrite(genome, index, std::byte(32));
	index++;
	GN::genomeWrite(genome, index, std::byte(33));
	index++;
	GN::genomeWrite(genome, index, std::byte(34));
	index++;
	GN::genomeWrite(genome, index, std::byte(36));
	index++;
	GN::genomeWrite(genome, index, std::byte(37));
	index++;
	//GN::genomeWrite(genome, index, std::byte(38));
	//index++;
	for (int i(0); i < 10; i++) {
		GN::genomeWrite(genome, index, std::byte(255 * (i%2)));
		index++;
	}


	// define payload and GeneDefinition for ANNLayer
	struct ANNLayer {
		double LayerPos;
		std::array<double, 10> weights;
	};
	AbstractGenome::GeneDefinition ANNLayer = { "ANNLayer",{(std::byte)13},sizeof(ANNLayer) };

	// define payload and GeneDefinition for deterministic gate
	struct DetGatePayload {
		int num_in, num_out;
		std::array<uint8_t, 7> addresses;
		std::bitset<17> logic; // will use 3 bytes
		std::uint8_t val;
	};
	AbstractGenome::GeneDefinition DetGeneDef = { "det",{(std::byte)42,(std::byte)212},sizeof(DetGatePayload) };


	// define payload and GeneDefinition for probabilistic gate
	struct ProbGatePayload {
		int num_in, num_out;
		std::array<uint8_t, 8> addresses;
		std::array<bool, 5> logic; // will use 16 bytes
	};
	AbstractGenome::GeneDefinition ProbGeneDef = { "prob",{(std::byte)43,(std::byte)211},sizeof(ProbGatePayload) };

	// set up genes set in genome
	int key = genome->initGeneSet({ DetGeneDef,ProbGeneDef });

	// get the list of start positions for the genes
	auto startPos = genome->getGenePositions(key);

	// read a DetGatePayload from start of first deterministic gate
	const auto& gate0_info = GN::genomeRead<DetGatePayload>(genome, 0);

	// print sizes
	std::cout << "Det GatePayload sizeof() " << sizeof(gate0_info) << std::endl;
	std::cout << "Det GatePayload.num_in sizeof() " << sizeof(gate0_info.num_in) << std::endl;
	std::cout << "Det GatePayload.num_in sizeof() " << sizeof(gate0_info.num_out) << std::endl;
	std::cout << "Det GatePayload.addresses sizeof() " << sizeof(gate0_info.addresses) << std::endl;
	std::cout << "Det GatePayload.logic sizeof() " << sizeof(gate0_info.logic) << std::endl;
	std::cout << "Det GatePayload.val sizeof() " << sizeof(gate0_info.val) << std::endl;

	std::cout << "Det Gate:" << std::endl;
	std::cout << "  num_in:  " << gate0_info.num_in << std::endl;
	std::cout << "  num_out: " << gate0_info.num_out << std::endl;
	std::cout << "  addresses: ";
	for (auto& v : gate0_info.addresses) {
		std::cout << " " << int(v);
	}
	std::cout << "\n  logic: ";
	for (size_t v(0); v < gate0_info.logic.size(); v++) {
		std::cout << " " << gate0_info.logic[v];
	}
	std::cout << std::endl;
	std::cout << "  val: " << int(gate0_info.val) << std::endl;

	// read a ProbGatePayload from start of first deterministic gate
	auto gate2_info = GN::genomeRead<ProbGatePayload>(genome, startPos["det"][0]);

	// print sizes
	std::cout << "ProbGatePayload sizeof() " << sizeof(gate2_info) << std::endl;
	std::cout << "ProbGatePayload.num_in sizeof() " << sizeof(gate2_info.num_in) << std::endl;
	std::cout << "ProbGatePayload.addresses sizeof() " << sizeof(gate2_info.addresses) << std::endl;
	std::cout << "ProbGatePayload.logic sizeof() " << sizeof(gate2_info.logic) << std::endl;

	std::cout << "Prob Gate:" << std::endl;
	std::cout << "  num_in:  " << gate2_info.num_in << std::endl;
	std::cout << "  num_out: " << gate2_info.num_out << std::endl;
	std::cout << "  addresses: ";
	for (auto& v : gate2_info.addresses) {
		std::cout << " " << int(v); // type is uint8_t so we need to make it an int to print
	}
	std::cout << "\n  logic: output as bool:";
	for (bool v : gate2_info.logic) {
		std::cout << " " << v;
	}
	std::cout << "\n\nthis is a problem... but..." << std::endl;
	std::cout << "\n  logic: output as (value & 1):";
	for (bool v : gate2_info.logic) {
		std::cout << " " << (v & 1);
	}
	std::cout << std::endl;
}

int main() {
	AbstractGenome* genome = new TestGenome(8);
	// AbstractGenome  & y = *(new TestGenome);

	runTests(genome);
	delete genome;

	AbstractGenome* secondGenome = new TestGenome(200);

	runGeneTest(secondGenome);
	delete secondGenome;


	return(0);
}
