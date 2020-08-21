# NEW_Genome_1.0
space to work on new genome class for MABE


### To Compile and Test:
```sh
cd code
cd build
```

The default genome is TestGenome. To run on a different genome, follow the instructions in the code/build folder first and then execute the commands below. 

**For running tests:**
```sh
bash build.sh Test
./test
```
**For running benchmarking:**
```sh
bash build.sh Bench
./bench
```

### Adding Tests:
Create test header under Test folder.

### Adding Genomes:
Create genome .cpp and .h in Genomes Folder. <br />
If you're adding a folder, add `include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Genomes/< foldername >)` under `# include source` in code/CMakeLists.txt.

### Changing Tests:
Include the testing header file name, and add the function calls in main.cpp.