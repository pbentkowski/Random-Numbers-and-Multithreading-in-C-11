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

        
};

#endif // RANDOM_H
