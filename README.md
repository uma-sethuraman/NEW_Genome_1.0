# NEW_Genome_1.0
space to work on new genome class for MABE


### Setting the Genome Type:
To set the genome type that is being tested or benchmarked, follow the instructions below. 

#### Option 1: Using cmake
```sh
cd code/build
cmake -DGenomeType=TestGenome ../Test
cmake --build ../Test
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

```sh
cmake --build ../Test
```

- For benchmarking, replace `../Test` with `../Bench`.

### To Run Tests:
- First, set the **genome type** to the desired genome (using instructions from the first section). Then use the commands below to run the tests on that genome type.
```sh
cd code/build
bash build.sh Test
./test
```

### To Run Benchmarking:
- First, set the **genome size** and **mutation rate** in the `Variables` section at the top of `bench.cpp` in the `code/Bench` folder. 
- Then, set the **genome type** to the desired genome (using instructions from the first section).
- Now, use the commands below to run benchmarking.
```sh
cd code/build
bash build.sh Bench
./bench
```
To view the benchmarking results, create a folder called `logs` inside the `code/build` folder. Then run `./bench` in the build folder. The benchmarking results will now be saved into GENOMETYPE.log in the logs folder.

### Adding Tests:
- Create the testing header file in the `code/Test` folder.
- Include the testing header file name and add the testing function calls in `main.cpp` in the Test folder. 
- In `main.cpp`, you can also change the arguments passed into testing function calls.

### Adding Genomes:
- Create genome .cpp and .h in the `code/Genomes` Folder. <br />
- If you're adding a folder, add `include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Genomes/< foldername >)` under `# include source` in the CMakeLists.txt.
