### How to use C++11 `<random>` with OpenMP `<omp.h>`

#### What is here?

Three files showing how to use it:

 - `Random.h` - header file for the library
 - `Random.cpp` - implementations of functions generating RNs from different distributions, but using the same engine
 - `getRandomMulti.cpp` - examples how to do it in a multi-threading environment


#### Compilation

Compiles with **g++ (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609** and runs e.g. by typing

```
mkdir bin
g++ -o bin/rands Random.cpp getRandomMulti.cpp -std=c++11 -fopenmp
./bin/rands
```

#### Reference

 1. Regarding the multi-threading programming in C++11 here's a guide into OpenMP: https://bisqwit.iki.fi/story/howto/openmp/

 1. For more tips how to use the `<random>` library in C++11 see: https://isocpp.org/files/papers/n3551.pdf
