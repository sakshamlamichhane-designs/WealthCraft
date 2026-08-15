#include "Random.h"

double Random::randomDouble(double minimum, double maximum)
{
    static std::mt19937 generator(std::random_device{}());

    std::uniform_real_distribution<double> distribution(minimum, maximum);

    return distribution(generator);
}