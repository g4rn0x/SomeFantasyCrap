#include "RandomGenerator.h"
#include <cstdlib>
#include <ctime>
#include <random>

static std::mt19937 g_generator;

int RandomGenerator::random(int min, int max)
{
    if (min > max) {
        std::swap(min, max);
    }
    if (min == max) {
        return min;
    }
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(g_generator);
}

double RandomGenerator::randomDouble()
{
    std::uniform_real_distribution<> distribution(0.0, 1.0);
    return distribution(g_generator);
}

void RandomGenerator::initializeSeed()
{
    std::random_device rd;
    g_generator.seed(rd());
}
