#include <random>

#ifndef RANDOM_H
#define RANDOM_H


/**
 * @brief A handy class for managing your daily pseudo-random number generation with C++11 STL <random> library
 *
 * For more tips how to use <random> in C++11 see:
 * https://isocpp.org/files/papers/n3551.pdf
 */
class Random{
    private:
        std::mt19937 m_mt;
    public:
        Random();
        ~Random();
        void reseed(uint32_t seed);
        class CustomProb {
            private:
                std::vector<float> probabils;
                std::vector<float> values;
                bool isOK;
            public:
                CustomProb();
                bool checkProbs();
                std::vector<float> getProbabils();
                float getOneValue(unsigned int indx);
                bool loadTheData(std::vector<float> probs, std::vector<float> vals);
                bool isCustomProbOK() { return isOK;}
        };
        //return a random unsigned int value in [min, max] from uniform distribution
        unsigned int getRandomFromUniform(unsigned int min, unsigned int max);
        //return a random float value in [0, 1) from uniform distribution
        float getUni();
        //return a random float value in from a user-defined gaussian distribution
        float getRandomFromGaussian(float mean, float variance);
        //return true with probability prob, in range [0,1);
        bool getBool(float prob);
        // return integer form [0, weights.size()] with weights proportional to ones given in the weights vector
        unsigned int getRandomIntegersWithWeights(std::vector<float> weights);

        float getValueAccordingToGivenProb(CustomProb probData);
        std::vector<float> getAlotOfValuesAccordingToGivenProb(CustomProb probData, unsigned int manySamples);
};

#endif // RANDOM_H
