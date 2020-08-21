# NEW_Genome_1.0
space to work on new genome class for MABE


### Setting the Genome Type:
The default genome is TestGenome. To change the genome type that is being tested or benchmarked, follow the instructions below. 

#### Option 1: Using cmake
```sh
cd code
cd build
cmake -DGenomeType=TestGenome ../Test
```
- Replace `TestGenome` with the genome type you want to test.
- For benchmarking, replace `../Test` with `../Bench`.

#### Option 2: Using the GUI
```sh
cd code
cd build
ccmake ../Test
```
where you [c]onfigure (scan the CMakeLists.txt files)
then change options as you see fit using arrow keys
then [g]enerate the makefile
then [q]uit
then make as normal.

- Replace `TestGenome` with the genome type you want to test.
- For benchmarking, replace `../Test` with `../Bench`.

### To Run Tests:
```sh
bash build.sh Test
./test
```

### To Run Benchmarking:
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
