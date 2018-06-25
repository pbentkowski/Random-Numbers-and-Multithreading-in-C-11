#include <iostream>    // for generic input/output
#include <fstream>     // for input/output on files
#include <time.h>      // for seeding the PRNG engine
#include <omp.h>       // for parallel computation
#include "Random.h"

 // Compiles with g++ (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609 by typing
 // g++ -o bin/rands Random.cpp getRandomMulti.cpp -std=c++11 -fopenmp

 // Regarding the multithreading programming in C++11  for a guide into OpenMP
 // see https://bisqwit.iki.fi/story/howto/openmp/

 // For more tips how to use <random> in C++11 see:
 // https://isocpp.org/files/papers/n3551.pdf

void runRandomz(int numberOfThreads, Random* randGen_ptr);

int main()
{
    // say here how many threads you want:
    int numberOfThreads = 3;
    omp_set_num_threads(numberOfThreads);

    // inits the PRNG engine class
    Random* rng;

    // creates the PRNG instances corresponding in number to the number of threads
    rng = new Random[numberOfThreads];

    // re-seeds the PRNG instances with different seeds, also selected (semi)randomly
    srand (static_cast<unsigned int>(time(nullptr)));
    int seed = rand();
    // int seed = 232323;  // or use the fixed seed
    for (int j = 0; j < numberOfThreads; ++j){
        rng[j].reseed(seed*(j+1));
    }

    // run the parallel random machinery
    runRandomz(numberOfThreads, rng);

    std::cout << "Done! Check the 'randomz_*.dat' files for output. "
              << std::endl;

    return 0;
}


// Here the parallel random stuff is being done
void runRandomz(int numberOfThreads, Random* randGen_ptr){
    // create the files where the data will go; one file for each thread
    std::ofstream randomFiles;
    for( int threadID = 0; threadID < numberOfThreads; ++threadID ) {
        std::string filename = "randomz_"   + std::to_string(threadID) + ".dat";
        randomFiles.open(filename, std::ios::out | std::ios::trunc);
        randomFiles << "#random_numbers\n";  // file header
        randomFiles.close();
    }
    // The parallel section starts here :
    #pragma omp parallel default(none) shared(randGen_ptr, std::cout)
    {
        std::ofstream randomzFile;
        #pragma omp for schedule(dynamic)
        for (unsigned int i = 1; i <= 9000; ++i ){
            if(!randomzFile.is_open()) {
                    std::string filenameRand = "randomz_" + std::to_string(omp_get_thread_num()) + ".dat";
                    randomzFile.open(filenameRand, std::ios::out | std::ios::ate | std::ios::app);
                }
                randomzFile << randGen_ptr[omp_get_thread_num()].getRandomFromUniform(0, 99) << std::endl;
        }
        if (randomzFile.is_open())
            randomzFile.close();
    }
    // The end of the` parallel section
}
