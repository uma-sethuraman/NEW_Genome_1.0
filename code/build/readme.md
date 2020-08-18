
Use the build script to build defaults, or use cmake to choose which genome type:

in the 'build' dir:
```sh
cmake -DGenomeType=TestGenome ../Bench
```
or
```sh
cmake -DGenomeType=TestGenome ../Test
```

or use the gui

```sh
ccmake ../Test
```

where you [c]onfigure (scan the CMakeLists.txt files)
then change options as you see fit using arrow keys
then [g]enerate the makefile
then [q]uit
then make as normal
