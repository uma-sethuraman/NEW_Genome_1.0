// stl library
#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include <bitset>
#include <random>
#include <map>
#include <cstddef>
#include "Tools/utilities.h"
#include "Genomes/TetianaGenome.h"

#define CATCH_CONFIG_RUNNER
#include "Tools/catch.hpp"

#include "Genomes/AbstractGenome.h"
#include "Genomes/TestGenome.h"
#include "Tests/UmaChangelogTests.h"

void testData(AbstractGenome* genome)
{
    for (size_t i(0); i < genome->size(); i++)
    {
        *genome->data(i) = (std::byte)(i*2);
        // std::cout << (int)*genome->data(i) << std::endl;
    }
    
    auto data = genome->data(2, 2);
    std::cout << (int)*data << std::endl;
    assert(*data == (std::byte)4);
    assert(*(data+1) == (std::byte)6);
    
}


void runTests(AbstractGenome* genome) {
    //initialize genome to 8 sites
    genome->resize(8);
    auto readHead = genome->data();
    for (size_t i(0); i < genome->size(); ++i)
        readHead[i] = (std::byte)(i + 1);
    readHead[7] = (std::byte)(0b10000001); // 129, or -127
    
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
    for (size_t i = 0; i < 4; i++) { // rewrite first 4 bytes with (2,4,6,8)
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
    
    std::cout << "\n\nrunGeneTest...\nThis shows an example of loading\na struct into the genome and\nhow memory is affected.\n" << std::endl;
    
    // define payload and GeneDefinition for deterministic gate
    struct MyGenePayload {
        int num_in, num_out;
        std::array<uint8_t, 7> addresses;
        std::bitset<17> logic; // will use 4 bytes, bitset uses 4 bytes at a time
        double answer;
    };
    
    AbstractGenome::GeneDefinition MyGeneDef = { "MyGene",{(std::byte)42,(std::byte)212},sizeof(MyGenePayload) };
    //GeneDefinition name, start codon, size of gene
    
    std::cout << "gene contains:\n  ints  'num_in', 'num_out'\n  array<uint8_t, 7> 'addresses'\n  bitset<17> 'logic'\n  double 'answer'";
    
    // write a MyGenomePayload into the genome at index 0
    MyGenePayload gene_instance;
    clean(gene_instance); // zero out memory
    gene_instance.num_in = 255+10;
    gene_instance.num_out = 20;
    gene_instance.addresses = {30,31,32,33,34,35,36};
    gene_instance.logic = 0b111100101100101101;
    gene_instance.answer = 42.42;
    GN::genomeWrite(genome, 0, gene_instance);
    // note that the bitstring will print in reverse order below (least signifigent diget first)
    
    // set up genes set in genome
    // the list can contain more then one GeneDefinition
    // key is a value used to get these genes
    int key = genome->initGeneSet({ MyGeneDef });
    
    // get the list of start positions for the genes
    auto startPos = genome->getGenePositions(key);
    
    // read a MyGene from start of first deterministic gate
    const auto& MyGene_data = GN::genomeRead<MyGenePayload>(genome, 0);
    
    // print sizes
    std::cout << "Det MyGene_data sizeof() " << sizeof(MyGene_data) << std::endl;
    std::cout << "Det MyGene_data.num_in sizeof() " << sizeof(MyGene_data.num_in) << std::endl;
    std::cout << "Det MyGene_data.num_out sizeof() " << sizeof(MyGene_data.num_out) << std::endl;
    std::cout << "Det MyGene_data.addresses sizeof() " << sizeof(MyGene_data.addresses) << std::endl;
    std::cout << "Det MyGene_data.logic sizeof() " << sizeof(MyGene_data.logic) << std::endl;
    std::cout << "Det MyGene_data.answer sizeof() " << sizeof(MyGene_data.answer) << std::endl;
    
    std::cout << "Det Gate:" << std::endl;
    std::cout << "  num_in:  " << MyGene_data.num_in << std::endl;
    std::cout << "  num_out: " << MyGene_data.num_out << std::endl;
    std::cout << "  addresses: ";
    for (auto& v : MyGene_data.addresses) {
        std::cout << " " << int(v);
    }
    std::cout << "\n  logic: ";
    for (size_t v(0); v < MyGene_data.logic.size(); v++) {
        std::cout << " " << MyGene_data.logic[v];
    }
    std::cout << std::endl;
    std::cout << "  val: " << MyGene_data.answer << std::endl;
    
    // iterate over MyGene_data size locations in genome
    auto head = genome->data();
    std::cout << "genome byte values for MyGene:" << std::endl;
    for (int i(0); i < sizeof(MyGenePayload); i++) {
        std::cout << " " << i << ":" << (int)head[i] << std::endl;
    }
    std::cout << std::endl;
    std::cout << "0-3 = num_in (int), 4-7 = num_out (int),\n"
    "8-14 = addresses (vect of uint8_t),"
    "\n15 is skipped so bitstring can start on word,\n"
    "16-23 logic (bitset), 24-31 anwser (double)\n" << std::endl;
}

void find_patterns_tests() {
    AbstractGenome* test_genome = new TestGenome(30);
    size_t i(0);
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(4) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(5) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(7) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(2) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(4) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(2) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(2) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(4) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(5) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(2) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(4) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(5) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(4) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(4) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    GN::genomeWrite<std::byte>({ .genome = test_genome, .index = i++, .value = std::byte(3) });
    
    std::vector<std::byte> pattern1({ (std::byte)2,(std::byte)3,(std::byte)4 });
    std::vector<std::byte> pattern2({ (std::byte)3,(std::byte)4,(std::byte)5,(std::byte)3 });
    std::vector<std::byte> pattern3({ (std::byte)3,(std::byte)3,(std::byte)3 });
    
    
    assert(test_genome->find_all(pattern1) == std::vector<size_t>({6, 10, 15}));
    assert(test_genome->find_all(pattern2) == std::vector<size_t>({ 0,11,16 }));
    assert(test_genome->find_all(pattern3) == std::vector<size_t>({ 21,22,26,27 }));
    
    std::cout << "all find pattern tests passed" << std::endl;
    /*
     for (auto& pat : std::vector< std::vector<std::byte>>({ pattern1,pattern2,pattern3 })) {
     std::cout << "looking for pattern: ";
     for (auto& v : pat) {
     std::cout << (int)v << " ";
     }
     std::cout << std::endl;
     auto gene_indexes = test_genome->find_all(pat);
     std::cout << "found locations: ";
     for (auto v : gene_indexes) {
     std::cout << v << " ";
     }
     std::cout << std::endl << std::endl;
     }
     */
}

void test_alterations() {
    std::cout << "----------------------------" << std::endl;
    std::cout << "-- begin test_alterations --" << std::endl;
    AbstractGenome* test_genome = new TestGenome(0);
    test_genome->show();
    test_genome->insert(0, std::vector<std::byte>{ (std::byte)1, (std::byte)2, (std::byte)3 });
    test_genome->show();
    test_genome->insert(0, std::vector<std::byte>{ (std::byte)1, (std::byte)2, (std::byte)3 });
    test_genome->show();
    test_genome->overwrite(2, std::vector<std::byte>{ (std::byte)4, (std::byte)4, (std::byte)4 });
    test_genome->show();
    test_genome->remove(1, 2);
    test_genome->show();
    std::cout << "-- done test_alterations --" << std::endl;
    std::cout << "---------------------------" << std::endl << std::endl;
}


void test_alterations_tetiana() {
    std::cout << "----------------------------" << std::endl;
    std::cout << "-- begin test_alterations_tetiana --" << std::endl;
    
    AbstractGenome* test_genome = new TetianaGenome(0);
    test_genome->show();
    test_genome->insert(0, std::vector<std::byte>{ (std::byte)1, (std::byte)2, (std::byte)3 });
    test_genome->show();
    test_genome->insert(0, std::vector<std::byte>{ (std::byte)1, (std::byte)2, (std::byte)3 });
    test_genome->show();
    
    std::cout << "-- done test_alterations_tetiana --" << std::endl;
    std::cout << "---------------------------" << std::endl << std::endl;
}

int main(int argc, char* argv[]) {
    
//    test_alterations();
//
//    find_patterns_tests();
//
//    AbstractGenome* genome = new TestGenome(8);
//    // AbstractGenome  & y = *(new TestGenome);
//
//    runTests(genome);
//    testData(genome);
//    delete genome;
//
    //AbstractGenome* secondGenome = new TestGenome(200);
    //runGeneTest(secondGenome);
    //delete secondGenome;
    
    
//    //=========================================================
//    AbstractGenome* myGenome = new TetianaGenome(11);
//
//    GN::genomeWrite<std::byte>(myGenome, 0, (std::byte)21);
//    GN::genomeWrite<std::byte>(myGenome, 1, (std::byte)35);
//    GN::genomeWrite<std::byte>(myGenome, 2, (std::byte)43);
//    GN::genomeWrite<std::byte>(myGenome, 3, (std::byte)84);
//    GN::genomeWrite<std::byte>(myGenome, 4, (std::byte)12);
//    GN::genomeWrite<std::byte>(myGenome, 5, (std::byte)15);
//    GN::genomeWrite<std::byte>(myGenome, 6, (std::byte)19);
//    GN::genomeWrite<std::byte>(myGenome, 7, (std::byte)28);
//    GN::genomeWrite<std::byte>(myGenome, 8, (std::byte)39);
//    GN::genomeWrite<std::byte>(myGenome, 9, (std::byte)45);
//    GN::genomeWrite<std::byte>(myGenome, 10, (std::byte)51);
//
//    myGenome->show();
//
//    myGenome->remove(3, 1);
//    myGenome->show();
//
//    std::vector<std::byte> segment1{
//        (std::byte)1
//    };
//    myGenome->insert(4, segment1);
//    myGenome->show();
//
//    std::cout << "myGenome->size():" << myGenome->size() << std::endl;

    
    
    
    
    
//
//    //=========================================================
//    AbstractGenome* genome = new TetianaGenome(10);
////    GN::genomeWrite<std::byte>(genome, 0, (std::byte)11);
////    GN::genomeWrite<std::byte>(genome, 1, (std::byte)22);
////    GN::genomeWrite<std::byte>(genome, 2, (std::byte)33);
////    GN::genomeWrite<std::byte>(genome, 3, (std::byte)44);
////    GN::genomeWrite<std::byte>(genome, 4, (std::byte)55);
////    GN::genomeWrite<std::byte>(genome, 5, (std::byte)66);
////    GN::genomeWrite<std::byte>(genome, 6, (std::byte)77);
////    GN::genomeWrite<std::byte>(genome, 7, (std::byte)88);
////    GN::genomeWrite<std::byte>(genome, 8, (std::byte)99);
////    GN::genomeWrite<std::byte>(genome, 9, (std::byte)111);
//
//    genome->resize(11);
//    GN::genomeWrite<std::byte>(genome, 0, (std::byte)21);
//    GN::genomeWrite<std::byte>(genome, 1, (std::byte)35);
//    GN::genomeWrite<std::byte>(genome, 2, (std::byte)43);
//    GN::genomeWrite<std::byte>(genome, 3, (std::byte)84);
//    GN::genomeWrite<std::byte>(genome, 4, (std::byte)12);
//    GN::genomeWrite<std::byte>(genome, 5, (std::byte)15);
//    GN::genomeWrite<std::byte>(genome, 6, (std::byte)19);
//    GN::genomeWrite<std::byte>(genome, 7, (std::byte)28);
//    GN::genomeWrite<std::byte>(genome, 8, (std::byte)39);
//    GN::genomeWrite<std::byte>(genome, 9, (std::byte)45);
//    GN::genomeWrite<std::byte>(genome, 10, (std::byte)51);
//
//    std::cout << "genome->size() init: " << genome->size() << std::endl;
//    genome->show();
//
////    genome->insert(7, std::vector<std::byte>({(std::byte)(212)}));
////    genome->show();
////
////    genome->remove(5, 3);
////    genome->show();
//
//
////    // insert {200, 250} starting at position 5
////    genome->insert(5, std::vector<std::byte>({(std::byte)(200), (std::byte)(250)}));
////    genome->show();
////
////    // remove positions 5,6,7,8
////    genome->remove(5, 4);
////    genome->show();
//
////    // insert {100, 110, 120, 130, 140} starting at position 6
////    genome->insert(6, std::vector<std::byte>({(std::byte)(100), (std::byte)(110), (std::byte)(120), (std::byte)(130), (std::byte)(140)}));
////    genome->show();
//
////    // remove positions 7 and 8
////    genome->remove(7, 2);
////    genome->show();
//
//
//
//
////    // insert value 212 at position 7
////    genome->insert(7, std::vector<std::byte>({(std::byte)(212)}));
////    genome->show();
////
////    // point mutation - change position 5's value to 200
////    genome->overwrite(5, std::vector<std::byte>({(std::byte)(200)}));
////    genome->show();
////
////    // insert {15,16} starting at position 2
////    genome->insert(2, std::vector<std::byte>({(std::byte)(15), (std::byte)(16)}));
////    genome->show();
////
////
////    // copy mutation: replace positions 1,2,3 with the last 3 values in the genome
////    std::vector<std::byte> copyVals(3);
////    int genIndex = genome->size()-copyVals.size();
////    for (int copyPos = 0; copyPos < copyVals.size(); copyPos++){
////        copyVals[copyPos] = GN::genomeRead<std::byte>(genome, genIndex);
////        genIndex++;
////    }
////    //11 88 99 111 33 44 55 200 77 212 88 99 111
////    genome->overwrite(1, copyVals);
////    genome->show();
//
//
//    genome->remove(3, 1);
//    genome->show();
//
//    std::vector<std::byte> segment1{
//        (std::byte)1
//    };
//    genome->insert(4, segment1);
//    genome->show();
//
//    genome->remove(6, 2);
//    genome->show();
//
//    std::vector<std::byte> segment2{
//        (std::byte)2,
//        (std::byte)3,
//        (std::byte)4
//    };
//    genome->insert(5, segment2);
//    genome->show();
//
//    genome->remove(6, 3);
//    genome->show();
//
//    genome->remove(6, 2);
//    genome->show();
//
//    std::vector<std::byte> segment3{
//        (std::byte)5,
//        (std::byte)6,
//        (std::byte)7
//    };
//    genome->insert(0, segment3);
//    genome->show();
//
//
//    std::cout << "test_offspring_data: " << std::endl;
//    std::cout << "genome->size(): " << genome->size() << std::endl;
//
//    std::byte* my_offspring_data = genome->data(0, 0);
//    for(int i = 0; i < genome->size(); i++) {
//        std::cout << (int)(*(my_offspring_data + i)) << " ";
//    }
//    std::cout << std::endl;
//
////    genome->resize(0);
//
////    GN::genomeWrite<std::byte>(genome, 0, (std::byte)21);
////    GN::genomeWrite<std::byte>(genome, 1, (std::byte)35);
////    GN::genomeWrite<std::byte>(genome, 2, (std::byte)43);
////    GN::genomeWrite<std::byte>(genome, 3, (std::byte)84);
////    GN::genomeWrite<std::byte>(genome, 4, (std::byte)12);
//

    
    
//    AbstractGenome* myGenome = new TetianaGenome(0);
//    myGenome->resize(0);
//    myGenome->show();
//
//    std::cout << "my_offspring_data: " << std::endl;
//    std::cout << "genome->size(): " << myGenome->size() << std::endl;
//
//    std::byte* my_offspring_data = myGenome->data(0, 0);
//    for(int i = 0; i < myGenome->size(); i++) {
//        std::cout << (int)(*(my_offspring_data + i)) << " ";
//    }
//    std::cout << std::endl;
//
//
//    std::cout << "insert: " << std::endl;
//    myGenome->insert(0, std::vector<std::byte>{ (std::byte)1, (std::byte)2, (std::byte)3 });
//    myGenome->show();
//
//    std::cout << "my_offspring_data: " << std::endl;
//    std::cout << "genome->size(): " << myGenome->size() << std::endl;
//    my_offspring_data = myGenome->data(0, 0);
//    for(int i = 0; i < myGenome->size(); i++) {
//        std::cout << (int)(*(my_offspring_data + i)) << " ";
//    }
//    std::cout << std::endl;
    
    
//    AbstractGenome* testGenome1 = new TestGenome(11);
//
//    GN::genomeWrite<std::byte>(testGenome1, 0, (std::byte)21);
//    GN::genomeWrite<std::byte>(testGenome1, 1, (std::byte)35);
//    GN::genomeWrite<std::byte>(testGenome1, 2, (std::byte)43);
//    GN::genomeWrite<std::byte>(testGenome1, 3, (std::byte)84);
//    GN::genomeWrite<std::byte>(testGenome1, 4, (std::byte)12);
//    GN::genomeWrite<std::byte>(testGenome1, 5, (std::byte)15);
//    GN::genomeWrite<std::byte>(testGenome1, 6, (std::byte)19);
//    GN::genomeWrite<std::byte>(testGenome1, 7, (std::byte)28);
//    GN::genomeWrite<std::byte>(testGenome1, 8, (std::byte)39);
//    GN::genomeWrite<std::byte>(testGenome1, 9, (std::byte)45);
//    GN::genomeWrite<std::byte>(testGenome1, 10, (std::byte)51);
//
//    AbstractGenome* testGenome2 = testGenome1->clone();
//
//    testGenome1->show();
//    testGenome2->show();
//
//    for (size_t i(0); i < testGenome1->size(); i++)
//    {
//        assert(*testGenome1->data(i) == *testGenome2->data(i));
//    }
//
//    assert(testGenome1->size() == testGenome2->size());
//    assert(*testGenome1->data(0,0) == *testGenome2->data(0,0));

    std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    runUmaChangelogTests<TetianaGenome>(0);
    std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
//

    int result = Catch::Session().run(argc, argv);
    return result;
}



