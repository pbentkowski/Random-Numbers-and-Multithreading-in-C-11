#include <random>
#include "Random.h"

// Create the PRNG engine and seed it using the random_device
Random::Random(): m_mt()
{
    m_mt.seed(std::random_device()());
}

Random::~Random()
{
}

//set the seed of random generator
void Random::reseed(unsigned int seed)
{
    m_mt.seed(seed);
}

//return a random unsigned int value in [min, max] from uniform distribution
unsigned int Random::getRandomFromUniform(unsigned int from, unsigned int thru)
{
    static std::uniform_int_distribution<unsigned int> d{};
    using parm_t = decltype(d)::param_type;
    return d( m_mt, parm_t{from, thru} );
}

//return a random float value in [0, 1) from uniform distribution
float Random::getUni()
{
    static std::uniform_real_distribution<float> d{};
    using parm_t = decltype(d)::param_type;
    return d( m_mt, parm_t{0., 1.} );
}

//return a random float value in from a user-defined gaussian distribution
float Random::getRandomFromGaussian(float mean, float variance)
{
    static std::normal_distribution<float> d{};
    using parm_t = decltype(d)::param_type;
    return d( m_mt, parm_t{mean, variance} );
}

bool Random::getBool(float prob){
    static std::uniform_real_distribution<float> d{};
    using parm_t = decltype(d)::param_type;
    return ( d( m_mt, parm_t{0., 1.} ) < prob );
}
