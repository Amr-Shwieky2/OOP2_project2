#include "RandomService.h"
#include <random>
#include <stdexcept>
#include <chrono>

//-------------------------------------------
// 1) Singleton accessor
//-------------------------------------------
RandomService& RandomService::instance() {
    static RandomService singleton;
    return singleton;
}

//-------------------------------------------
// 2) Constructor (seeds engine once)
//-------------------------------------------
RandomService::RandomService() {
    seedEngine();
}

//-------------------------------------------
// 3) seedEngine
//-------------------------------------------
//
// Seed the mt19937 engine using a high‐resolution clock or random_device.
//-------------------------------------------
void RandomService::seedEngine() {
    // Try to use std::random_device if available, otherwise fallback to time‐based seed
    std::random_device rd;
    std::seed_seq seq{
        static_cast<unsigned>(rd()),
        static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count())
    };
    m_engine.seed(seq);
}

//-------------------------------------------
// 4) getInt(min, max)
//-------------------------------------------
int RandomService::getInt(int min, int max) {
    if (min > max) {
        throw std::invalid_argument("RandomService::getInt(): min > max");
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    std::uniform_int_distribution<int> dist(min, max);
    return dist(m_engine);
}

//-------------------------------------------
// 5) getFloat(min, max)
//-------------------------------------------
float RandomService::getFloat(float min, float max) {
    if (min >= max) {
        throw std::invalid_argument("RandomService::getFloat(): min >= max");
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    std::uniform_real_distribution<float> dist(min, max);
    return dist(m_engine);
}

//-------------------------------------------
// 6) getBool(p)
//-------------------------------------------
bool RandomService::getBool(double p) {
    if (p < 0.0 || p > 1.0) {
        throw std::invalid_argument("RandomService::getBool(): probability must be in [0.0, 1.0]");
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    std::bernoulli_distribution dist(p);
    return dist(m_engine);
}
