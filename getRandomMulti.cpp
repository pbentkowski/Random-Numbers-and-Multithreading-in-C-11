#include <iostream>    // for generic input/output
#include <fstream>     // for input/output on files
#include <ctime>      // for seeding the PRNG engine
#include <omp.h>       // for parallel computation/**/
#include "Random.h"

 // Compiles with g++  by typing
 //
 // g++ -o rands Random.cpp getRandomMulti.cpp -std=c++11 -fopenmp
 // ./rands

 // Regarding the multithreading programming in C++11  for a guide into OpenMP
 // see https://bisqwit.iki.fi/story/howto/openmp/

 // For more tips how to use <random> in C++11 see:
 // https://isocpp.org/files/papers/n3551.pdf

// Function declarations
void simpleRandomRunz        (int numberOfThreads, Random *randGen_ptr);
void preDefinDistrRandomRunz (int numberOfThreads, std::vector<float> weights, Random *randGen_ptr);
void customRandomRunz        (int numberOfThreads, Random::CustomProb probsAndVals, Random *randGen_ptr);


int main()
{
    // !!! say here how many threads you want:
    int numberOfThreads = 3;
    omp_set_num_threads(numberOfThreads);

    // !!! inits the PRNG engine class
    Random* rng;

    // !!! creates the PRNG instances corresponding in number to the number of threads
    rng = new Random[numberOfThreads];

    // re-seeds the PRNG instances with different seeds, also selected (semi)randomly
    // If you skip it, the PRNG will be initialised from the std::random_device()
    srand (static_cast<unsigned int>(time(nullptr)));
    int seed = rand();
    // int seed = 232323;  // or use the fixed seed
    for (int j = 0; j < numberOfThreads; ++j){
        rng[j].reseed(static_cast<uint32_t>(seed * (j + 1)));
    }

    // run the parallel random machinery - simple stuff
    // Parallelism is built into the function below
//    simpleRandomRunz(numberOfThreads, rng);

    // run the parallel random machinery -  [0, len(n)-1] integers according to weights in a vector
//    std::vector<float> weights = {0.1, 0.5, 0.5, 0.1};
    // Parallelism is built into the function below
//    preDefinDistrRandomRunz(numberOfThreads, weights, rng);

     // run the parallel random machinery -  values according to probability distribution in a vector
    std::vector<float> probs = {0.1, 0.2, 0.3, 0.2, 0.1, 0.1};
    std::vector<float> vals =  {1.5, 2.5, 3.0, 3.5, 4.5, 5.5};
    Random::CustomProb probsAndVals;
    probsAndVals.loadTheData(probs, vals);
    // Parallelism is built into the function below
    customRandomRunz(numberOfThreads, probsAndVals, rng);

    std::cout << "Done! Check the 'randomz_*.dat' files for output. "
              << std::endl;

    exit(0);
}

// =====================================================================================================

/**
 * @brief Generates the `numberOfThreads` of files with random integers between 0 and 99 (inclusive)
 *
 * @param numberOfThreads - how many threads are there (must check this carefully)
 * @param randGen_ptr - the array of pointers to the PRNG instances
 */
void simpleRandomRunz(int numberOfThreads, Random *randGen_ptr){
    // create the files where the data will go; one file for each thread
    std::ofstream randomFiles;
    std::string filename;
    for( int threadID = 0; threadID < numberOfThreads; ++threadID ) {
        filename = "randomz_"   + std::to_string(threadID) + ".dat";
        randomFiles.open(filename, std::ios::out | std::ios::trunc);
        randomFiles << "#random_numbers\n";  // file header
        randomFiles.close();
    }
    // The parallel section starts here :
    #pragma omp parallel default(none) shared(randGen_ptr, std::cout)
    {
        std::ofstream randomzFile;
        #pragma omp for schedule(dynamic)
        for( unsigned int i = 1; i <= 10000; ++i ){
            if(!randomzFile.is_open()) {
                    std::string filenameRand = "randomz_" + std::to_string(omp_get_thread_num()) + ".dat";
                    randomzFile.open(filenameRand, std::ios::out | std::ios::ate | std::ios::app);
                }
                // this is the line you're looking for:
                randomzFile << randGen_ptr[omp_get_thread_num()].getRandomFromUniform(0, 99) << std::endl;
        }
        if (randomzFile.is_open())
            randomzFile.close();
    }
    // The end of the parallel section
}


/**
 * @brief Takes a vector of weights and generates the `numberOfThreads` of files with random integers between 0 and
 * len(weights)-1 with accordance to the weights given in the vector.
 *
 * The weights vector contains WEIGHTS, not probabilities. It's elements do not have to sum to 1.0.
 *
 * @param numberOfThreads - how many threads are there (must check this carefully)
 * @param weights - the weights STL vector, elements don't have to sum to 1.0
 * @param randGen_ptr - the array of pointers to the PRNG instances
 */
void preDefinDistrRandomRunz(int numberOfThreads, std::vector<float> weights, Random *randGen_ptr)
{
    // create the files where the data will go; one file for each thread
    std::ofstream randomFiles;
    std::string filename;
    for( int threadID = 0; threadID < numberOfThreads; ++threadID ) {
        filename = "randomz_"   + std::to_string(threadID) + ".dat";
        randomFiles.open(filename, std::ios::out | std::ios::trunc);
        randomFiles << "#random_numbers\n";  // file header
        randomFiles.close();
    }
    // The parallel section starts here :
    #pragma omp parallel default(none) shared(randGen_ptr, weights, std::cout)
    {
        std::ofstream randomzFile;
        #pragma omp for schedule(dynamic)
        for( unsigned int i = 1; i <= 10000; ++i ){
            if(!randomzFile.is_open()) {
                    std::string filenameRand = "randomz_" + std::to_string(omp_get_thread_num()) + ".dat";
                    randomzFile.open(filenameRand, std::ios::out | std::ios::ate | std::ios::app);
                }
                // this is the line you're looking for:
                randomzFile << randGen_ptr[omp_get_thread_num()].getRandomIntegersWithWeights(weights) << std::endl;
        }
        if (randomzFile.is_open())
            randomzFile.close();
    }
    // The end of the parallel section
}


/**
 * @brief Takes a `Random::CustomProb` data structure and generates values from it with probabilities defined by
 * a custom probability distribution
 *
 * @param numberOfThreads - how many threads are there (must check this carefully)
 * @param probsAndVals - a data structure holding values and corresponding probability distribution
 * @param randGen_ptr  - the array of pointers to the PRNG instance
 */
void customRandomRunz(int numberOfThreads, Random::CustomProb probsAndVals, Random *randGen_ptr)
{
    // create the files where the data will go; one file for each thread
    std::ofstream randomFiles;
    std::string filename;
    for( int threadID = 0; threadID < numberOfThreads; ++threadID ) {
        filename = "randomz_"   + std::to_string(threadID) + ".dat";
        randomFiles.open(filename, std::ios::out | std::ios::trunc);
        randomFiles << "#random_numbers\n";  // file header
        randomFiles.close();
    }
    // The parallel section starts here :
    #pragma omp parallel default(none) shared(randGen_ptr, probsAndVals, std::cout)
    {
        std::ofstream randomzFile;
        #pragma omp for schedule(dynamic)
        for( unsigned int i = 1; i <= 10000; ++i ){
            if(!randomzFile.is_open()) {
                    std::string filenameRand = "randomz_" + std::to_string(omp_get_thread_num()) + ".dat";
                    randomzFile.open(filenameRand, std::ios::out | std::ios::ate | std::ios::app);
                }
                // this is the line you're looking for:
                randomzFile << randGen_ptr[omp_get_thread_num()].getValueAccordingToGivenProb(probsAndVals) << std::endl;
        }
        if (randomzFile.is_open())
            randomzFile.close();
    }
    // The end of the parallel section
}
