#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace Shared {
    class Random {
    public:
        static int getInt(int min, int max);
        static float getFloat(float min, float max);
        static double getDouble(double min, double max);

    private:
        static std::mt19937 rng;
    };
}

#endif
