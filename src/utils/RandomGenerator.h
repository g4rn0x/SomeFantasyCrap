#pragma once

#include <QDebug>

/**
 * @brief RandomGenerator - Utility for random number generation
 */
class RandomGenerator {
public:
    // Delete constructor - static utility class
    RandomGenerator() = delete;

    // Generate random integer in range [min, max]
    static int random(int min, int max);

    // Generate random double in range [0.0, 1.0]
    static double randomDouble();

    // Initialize random seed (call once at application start)
    static void initializeSeed();
};
