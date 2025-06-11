#pragma once

#include <random>
#include <mutex>

/**
 * RandomService is a singleton that provides:
 *   • Random integers in [min, max]
 *   • Random floats in [min, max)
 *   • Random booleans with a given probability
 *
 * Usage:
 *   int x = RandomService::instance().getInt(1, 10);
 *   float f = RandomService::instance().getFloat(0.0f, 1.0f);
 *   bool b = RandomService::instance().getBool(0.25); // 25% chance true
 */
class RandomService {
public:
    // Retrieve the sole instance
    static RandomService& instance();

    // Delete copy/move so only one instance exists
    RandomService(const RandomService&) = delete;
    RandomService& operator=(const RandomService&) = delete;
    RandomService(RandomService&&) = delete;
    RandomService& operator=(RandomService&&) = delete;
    ~RandomService() = default;


    /**
     * Returns a uniformly distributed integer in [min, max].
     *
     * Throws std::invalid_argument if min > max.
     */
    int getInt(int min, int max);

    /**
     * Returns a uniformly distributed float in [min, max).
     *
     * Throws std::invalid_argument if min >= max.
     */
    float getFloat(float min, float max);

    /**
     * Returns true with probability p (0.0 ≤ p ≤ 1.0), false otherwise.
     *
     * Throws std::invalid_argument if p < 0.0 or p > 1.0.
     */
    bool getBool(double p = 0.5);

private:
    RandomService();

    std::mt19937                 m_engine;       // Mersenne Twister engine
    std::mutex                   m_mutex;        // guards the engine

    // Helper: ensure the engine is seeded once
    void seedEngine();
};
