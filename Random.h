#include <random>

#ifndef RANDOM_H
#define RANDOM_H

class Random{
    private:
        std::mt19937 m_mt;
    public:
        Random();
        ~Random();
        void reseed(uint32_t seed);
        //return a random unsigned int value in [min, max] from uniform distribution
        unsigned int getRandomFromUniform(unsigned int min, unsigned int max);
        //return a random float value in [0, 1) from uniform distribution
        float getUni();
        //return a random float value in from a user-defined gaussian distribution
        float getRandomFromGaussian(float mean, float variance);
        //return true with probability prob, in range [0,1);
        bool getBool(float prob);

        //return a random value in [0, size-1] according to the distribution in dist
        unsigned int getRandomFromDist(float *dist, unsigned int size);
        //return a random value in [min, max] according to the distribution in dist
        unsigned int getRandomFromDist(float *dist, unsigned int size, unsigned int min, unsigned int max);
        //return a random value in [0, dist.size()] according to the distribution in dist
        unsigned int getRandomFromDist(const std::vector<float> &dist);
        //return a random value in [min, max] according to the distribution in dist
        unsigned int getRandomFromDist(const std::vector<float> &dist, unsigned int min, unsigned int max);

};

#endif // RANDOM_H
