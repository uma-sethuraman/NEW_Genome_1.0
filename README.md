# NEW_Genome_1.0
space to work on new genome class for MABE


### Setting the Genome Type:
The default genome is TestGenome. To change the genome type that is being tested or benchmarked, follow the instructions below. 

#### Option 1: Using cmake
```sh
cd code/build
cmake -DGenomeType=TestGenome ../Test
```
- Replace `TestGenome` with the genome type you want to test.
- For benchmarking, replace `../Test` with `../Bench`.

#### Option 2: Using the GUI
```sh
cd code/build
ccmake ../Test
```
where you [c]onfigure (scan the CMakeLists.txt files)
then change options as you see fit using arrow keys
then [g]enerate the makefile
then [q]uit
then make as normal.

- For benchmarking, replace `../Test` with `../Bench`.

### To Run Tests:
```sh
cd code/build
bash build.sh Test
./test
```

### To Run Benchmarking:
```sh
cd code/build
bash build.sh Bench
./bench
```
To view the benchmarking results, create a folder called `logs` inside the `code/build` folder. Then run `./bench` in the build folder. The benchmarking results will now be outputted to log files in the logs folder.

### Adding Tests:
Create test header under Test folder.

### Adding Genomes:
Create genome .cpp and .h in Genomes Folder. <br />
If you're adding a folder, add `include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Genomes/< foldername >)` under `# include source` in the CMakeLists.txt.

### Changing Tests:
Include the testing header file name, and add the function calls in main.cpp.
