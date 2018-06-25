#include <random>
#include <stdexcept>
#include "Random.h"

/**
 * @brief Constructor. Creates the PRNG engine and seeds it using the std::random_device
 */
Random::Random(): m_mt(std::mt19937(13637))
{
    m_mt.seed(std::random_device()());
}

/**
 * @brief Just a destructor
 */
Random::~Random()
{
}

/**
 * @brief Re-seed the PRNG machinery with a new seed
 *
 * @param seed - some number, a positive integer
 */
void Random::reseed(unsigned int seed)
{
    m_mt.seed(seed);
}


/**
 * @brief Returns a random unsigned int value in [min, max] from uniform distribution
 *
 * @param from - lower bound (inclusive)
 * @param thru - upper bound (inclusive)
 * @return positive intiger from from through thru
 */
unsigned int Random::getRandomFromUniform(unsigned int from, unsigned int thru)
{
    static std::uniform_int_distribution<unsigned int> d{};
    using parm_t = decltype(d)::param_type;
    return d( m_mt, parm_t{from, thru} );
}


/**
 * @brief Returns a random float value in [0, 1) from uniform distribution
 *
 * @return a float between 0. (inclusive) and 1. (exclusive)
 */
float Random::getUni()
{
    static std::uniform_real_distribution<float> d{};
    using parm_t = decltype(d)::param_type;
    return d( m_mt, parm_t{0., 1.} );
}


/**
 * @brief Returns a random float value from a user-defined gaussian distribution
 *
 * @param mean - mean of the normal distribution
 * @param variance - the variance the normal distribution
 * @return a float according to the normal distribution defined
 */
float Random::getRandomFromGaussian(float mean, float variance)
{
    static std::normal_distribution<float> d{};
    using parm_t = decltype(d)::param_type;
    return d( m_mt, parm_t{mean, variance} );
}


/**
 * @brief Returns true or false with proportion to a user-define threshold probability
 *
 * @param prob - threshold probability
 * @return  true with probability `prob` or false with `1 - prop`
 */
bool Random::getBool(float prob){
    static std::uniform_real_distribution<float> d{};
    using parm_t = decltype(d)::param_type;
    return ( d( m_mt, parm_t{0., 1.} ) < prob );
}


/**
 * @brief Returns a positive integer between 0 and the size of the `weights` vector according to weights
 * given in this vector.
 *
 * @param weights vector with weights (doesn't have to sum to 1.0 - these are not probabilities, but weights)
 * @return a positive integer
 */
unsigned int Random::getRandomIntegersWithWeights(std::vector<float> weights)
{
//    std::vector<float> weights = {0.1, 0.5, 0.5, 0.1, 0.3};
    static std::discrete_distribution<unsigned int> d2(weights.begin(), weights.end());
    return d2(m_mt);
}


// ================================================================================================================== //

// return a random value in [min, max] according to the distribution in dist
unsigned int Random::getRandomFromDist(const std::vector<float> &dist, unsigned int min, unsigned int max)
{
    if( dist.size() == 1 )
        return dist[0];
    if( min == max )
        return dist[min];
    float rr;
    static std::uniform_real_distribution<float> d{};
    using parm_t = decltype(d)::param_type;
    if(min == 0){
        rr = d( m_mt, parm_t{0., dist[max]} );
    } else {
        rr = d( m_mt, parm_t{dist[min-1], dist[max]} );
    }
    unsigned int value;
    for( value = min; rr > dist[value]; value++ );
    if(value >= dist.size())
        throw std::logic_error("Random: distribution overflow");
    return value;
}

//return a random value in [0, dist.size()-1] according to the distribution in dist
unsigned int Random::getRandomFromDist(const std::vector<float> &dist)
{
    return getRandomFromDist(dist, 0, dist.size()-1);
}


// return a random value in [min, max] according to the distribution in dist
unsigned int Random::getRandomFromDist(float *dist, unsigned int size, unsigned int min, unsigned int max)
{
    if( min == max )
        return dist[min];
    float rr;
    static std::uniform_real_distribution<float> d{};
    using parm_t = decltype(d)::param_type;
    if(min == 0){
        rr = d( m_mt, parm_t{0., dist[max]} );
    } else {
        rr = d( m_mt, parm_t{dist[min-1], dist[max]} );
    }
    unsigned int value;
    for( value = min; rr > dist[value]; value++ );
    if( value >= size )
        throw std::logic_error("Random: distribution overflow");
    return value;
}

//return a random value in [0, size-1] according to the distribution in dist
unsigned int Random::getRandomFromDist(float *dist, unsigned int size)
{
    return getRandomFromDist(dist, size, 0, size-1);
}
