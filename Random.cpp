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
= default;

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
    if( from == thru )
        return from;
    if( from > thru ){
        unsigned int tmp = from;
        from = thru;
        thru = tmp;
    }
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
    return d( m_mt, parm_t{0, 1} );
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
    return ( d( m_mt, parm_t{0, 1} ) < prob );
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


/**
 * @brief Takes the Custom Probability Data Structure and select a value in correspondence to the probability
 * distribution given.
 *
 * @param probData - CustomProb data structure with vectors of values and corresponding probabilities
 * @return - one, randomly selected (in accordance to probability distribution given) value
 */
float Random::getValueAccordingToGivenProb(CustomProb probData) {
    if (probData.isCustomProbOK()) {
        std::vector<float> tmpPros = probData.getProbabils();
        std::discrete_distribution<unsigned int> d3(tmpPros.begin(), tmpPros.end());
        unsigned int rr = d3(m_mt);
        return probData.getOneValue(rr);
    } else {
        throw std::logic_error("ERROR in Random::getValueAccordingToGivenProb(): The probability and value data format is incorrect");
    }
}


std::vector<float> Random::getAlotOfValuesAccordingToGivenProb(Random::CustomProb probData, unsigned int manySamples) {
    std::vector<float> randomz;
    if( manySamples == 1 ) {
        randomz.push_back(getValueAccordingToGivenProb(probData));
        return randomz;
    }
}


Random::CustomProb::CustomProb() {
    probabils.clear();
    values.clear();
    isOK = false;
}


/**
 * @brief Checks if the vector of probability distribution is equal in size to the vector of values and if it sums to 1.0
 *
 * @return - true if the probabilities vector makes sense as probabilities
 */
bool Random::CustomProb::checkProbs(){
    if (probabils.size() != values.size())
        throw std::logic_error("ERROR in CustomProb: Probability and value vectors are of unequal lengths:\nprobals.size() = "
                               + std::to_string(probabils.size()) + " , values.size() = " + std::to_string(values.size()));
    float sumProbs = 0.0;
    for (float &probabil : probabils) {
        sumProbs += probabil;
    }
    if (sumProbs != 1.0)
        throw std::logic_error("ERROR in CustomProb: The probabilities do not sum to 1.0! They sum to "
                               + std::to_string(sumProbs));
    isOK = true;
    return isOK;
}

/**
 * @brief Loads two vectors: probability distribution and values to a CustomProb data structure. Does basic data
 * consistency checks.
 *
 * @param probs - vector of probabilities (has to sum to 1.0)
 * @param vals - vector of the corresponding values
 * @return - true if data were loaded successfully
 */
bool Random::CustomProb::loadTheData(std::vector<float> probs, std::vector<float> vals) {
    if (probs.empty())
         throw std::logic_error("ERROR in CustomProb: Probability vector size is 0");
    if (vals.empty())
         throw std::logic_error("ERROR in CustomProb: Values vector size is 0");
    probabils = probs;
    values = vals;
    return checkProbs();
}


std::vector<float> Random::CustomProb::getProbabils() {
    return probabils;
}

float Random::CustomProb::getOneValue(unsigned int indx) {
    if (indx > values.size())
        throw std::logic_error("ERROR in getOneValue(): trying to access the value vector out-off-bounds");
    else {
        return values[indx];
    }
}