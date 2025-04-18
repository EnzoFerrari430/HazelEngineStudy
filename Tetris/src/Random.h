#pragma once

#include <random>
#include <cmath>

class Random {
public:
    static void Init() {
        s_RandomEngine.seed(std::random_device()());
    }

    static float Float()
    {
        return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
    }

    static int Int(int min, int max)
    {
        int length = max - min + 1;
        int result = std::abs((int)s_Distribution(s_RandomEngine));
        return (result % length) + min;
    }

private:
    static std::mt19937 s_RandomEngine;
    static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};