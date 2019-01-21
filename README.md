### How to use C++11 *random* with OpenMP *omp.h*

#### What is here?

Three files showing how to use it:

 - `Random.h` - header file for the library
 - `Random.cpp` - implementations of functions generating RNs from different distributions, but using the same engine
 - `getRandomMulti.cpp` - examples how to do it in a multi-threading environment
  `mutatorTest.cpp` - simple program scaling 'whole-bitsting mutation' to 'point mutations' in binary strings and checking if the scaling gives the same fraction of altered bitsting https://ndownloader.figshare.com/files/2546683


#### Compilation

Compiles with **g++ 5.4.0** (Ubuntu 5.4.0-6ubuntu1~16.04.9) and runs e.g. by typing

```
g++ -o rands Random.cpp getRandomMulti.cpp -O2 -std=c++11 -fopenmp
./rands
```

You can check the results in Ipython:

```python
%pylab
ww0 = np.genfromtxt("randomz_0.dat", skip_header=1)
ww1 = np.genfromtxt("randomz_1.dat", skip_header=1)
ww2 = np.genfromtxt("randomz_2.dat", skip_header=1)
hh = plt.hist(np.hstack((ww0, ww1, ww2)))
```


#### Reference

 1. Regarding the multi-threading programming in C++11 here's a guide into OpenMP: https://bisqwit.iki.fi/story/howto/openmp/
 1. For more tips how to use the `<random>` library in C++11 see: https://isocpp.org/files/papers/n3551.pdf
 1. For using *std::discrete\_distribution* with user defined weights: https://en.cppreference.com/w/cpp/numeric/random/piecewise_constant_distribution
