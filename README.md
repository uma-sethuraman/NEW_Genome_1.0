# NEW_Genome_1.0
Developing an optimized genome data structure for MABE (a platform for evolving and analyzing digital organisms)


## Initialization and Build Configuration:
If you haven't already cloned this repository to your local machine, do so with the following command:
```sh
git clone https://github.com/uma-sethuraman/NEW_Genome_1.0 newgenome
```
Now, move into the `code` directory in this repository with the following commands:
```sh
cd newgenome
cd code
```
**If this is your first time configuring and you don't already have a build directory, use the following commands:**
```sh
mkdir build
cd build
```
**Else if you are rebuilding with a new configuration and already have a build directory, you must remove all files in the build directory first. Use the following commands to do so:**
```sh
cd build
rm -rf *
```

## Setting the Genome Type:
Follow one of the options below to set the genome type that is being tested or benchmarked. 

#### Option 1: Using cmake
In the `code/build` directory, execute:
```sh
cmake -DGenomeType=TestGenome ../Test
```
- Replace `TestGenome` with the genome type you want to test.
- For benchmarking, replace `../Test` with `../Bench`.

#### Option 2: Using the GUI
In the `code/build` directory, execute:
```sh
ccmake ../Test
```
- In the GUI, you [c]onfigure (scan the CMakeLists.txt files) then change options as you see fit using arrow keys then [g]enerate the makefile then [q]uit then make as normal.
- For benchmarking, replace `../Test` with `../Bench`.

## Running Tests:
- **Step 1:** follow the instructions from the "Initialization and Build Configuration" section to configure the build folder. If rebuilding with a new configuration, please remember to remove all files in the build folder.
- **Step 2:** set the genome type to the desired genome using the instructions from the "Setting the Genome Type" section. 
- **Step 3**: use the commands below to run the tests on the set genome type. Don't execute the first command below if you're already in the build directory.
```sh
cd code/build                 
cmake --build .
./test
```

## Running Benchmarking:
- **Step 1:** follow the instructions from the "Initialization and Build Configuration" section to configure the build folder. If rebuilding with a new configuration, please remember to remove all files in the build folder.
- **Step 2:** set the genome size and mutation rate in the `Variables` section at the top of `bench.cpp` in the `code/Bench` folder. 
- **Step 3:** set the genome type to the desired genome using the instructions from the "Setting the Genome Type" section. 
- **Step 4:**: create a folder called `logs` inside the `code/build` folder (if `logs` doesn't already exist there). The benchmarking results will be saved in a log file in the logs folder.
- **Step 5:** use the commands below to run benchmarking on the set genome type. Don't execute the first command below if you're already in the build directory.
```sh
cd code/build
cmake --build .
./bench
```
To view the benchmarking results, open GENOMETYPE.log in the `code/build/logs` folder.

## Adding Tests:
- Create the testing header file in the `code/Test` folder.
- Include the testing header file name and add the testing function calls in `main.cpp` in the Test folder. 
- In `main.cpp`, you can also change the arguments passed into testing function calls.

## Adding Genomes:
- Create genome .cpp and .h in the `code/Genomes` Folder.
- If you're adding a folder, add `include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Genomes/< foldername >)` under `# include source` in the CMakeLists.txt.
