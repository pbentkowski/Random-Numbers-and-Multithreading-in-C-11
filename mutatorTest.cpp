#include <iostream>
#include <omp.h>
#include "boost/dynamic_bitset.hpp"
#include <vector>

#include "Random.h"

// Compile like this:
// g++ -o mutTest -O3 mutatorTest.cpp Random.cpp -fopenmp -std=c++14

// Mutation in bitstring - poin mutations
ulong mutatePoint(double pm_mut_probabl, boost::dynamic_bitset<> bitgene, Random& randGen){
    boost::dynamic_bitset<>::size_type bitgeneSize = bitgene.size();
    for(boost::dynamic_bitset<>::size_type i = 0; i < bitgeneSize; ++i) {
        if(randGen.getUni() < pm_mut_probabl) {
            bitgene[i].flip();
        }
    }
//    std::cout << bitgene << "\n" << std::flush;
    return  bitgene.to_ulong();
}

// Mutation in bitstring - whole-bitstring mutation
ulong mutateWhole(double mut_probabl, boost::dynamic_bitset<> bitgene, Random& randGen){
    if(randGen.getUni() < mut_probabl)
            bitgene.flip();
//    std::cout << bitgene << "\n" << std::flush;
    return  bitgene.to_ulong();
}

// function scaling 'whole bitstring mutations' probability to 'point mutation' probability
double MMtoPMscaling(double MM_prob_mut, unsigned long geneLength){
    double bitt = (double) geneLength;
    return 1.0 - std::exp((1.0 / bitt) * std::log(1.0 - MM_prob_mut * (1.0 - std::pow(0.5, bitt))));
}

int main() {
    // 'intput' parameters
    unsigned int NN = 100000000;
    unsigned long geneLength = 16;
    double mut_probabl = 0.0001;
    // scaling to the point poin mutation probability
    double pm_mut_probabl = MMtoPMscaling(mut_probabl, geneLength);
    boost::dynamic_bitset<> bitgene(geneLength, 0);  // set ZERO bitstring

    // setting-up the random number generator
    int numberOfThreads = 3;
    omp_set_num_threads(numberOfThreads);
    Random* rng;
    rng = new Random[numberOfThreads];
    for (int j = 0; j < numberOfThreads; ++j)
        rng[j].reseed(std::random_device()());

    // Print the inputs
    std::cout << "The initial bitgene is: " << bitgene << std::endl;
    std::cout << "Number of trials: " << NN/1000000 << " million" << std::endl;
    std::cout << "Whole mutation prob. is: " << mut_probabl << std::endl;
    std::cout << "Point mutation prob. is: " << pm_mut_probabl << std::endl;

    // testing point mutations
    unsigned int sum = 0;
    #pragma omp parallel for shared(rng, pm_mut_probabl, bitgene, NN) reduction (+:sum)
    for(unsigned int i = 0; i < NN; ++i) {
        // check if bitstring still evaluates to ZERO after mutating it (if mutated...)
        if(mutatePoint(pm_mut_probabl, bitgene, rng[omp_get_thread_num()]) != 0)
            sum = sum + 1;
    }
    double frac = (double)sum / (double)NN;
    std::cout << "The fraction of mutated in 'point mut.': " << frac << std::endl;

    // testing whole bistring mutations
    sum = 0;
    #pragma omp parallel for shared(rng, pm_mut_probabl, bitgene, NN) reduction (+:sum)
    for(unsigned int i = 0; i < NN; ++i) {
         // check if bitstring still evaluates to ZERO after mutating it (if mutated...)
         if(mutateWhole(mut_probabl, bitgene, rng[omp_get_thread_num()]) != 0)
            sum = sum + 1;
    }
    frac = (double)sum / (double)NN;
    std::cout << "The fraction of mutated in 'whole mut.': " << frac << std::endl;

    return 0;
}
