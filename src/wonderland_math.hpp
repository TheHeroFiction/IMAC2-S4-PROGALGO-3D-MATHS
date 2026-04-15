/* =====================================================================
 * WONDERLAND ENGINE
 * Math & Probability Generation
 * Note: This module was structured with the assistance of an AI
 * to correctly implement the probability distributions required.
 * ===================================================================== */

#pragma once

#include <random>
#include <vector>

// Engine responsible for generating various probability distributions
// exclusively from a uniform distribution [0, 1], as required by the assignment.
class WonderlandEngine {
private:
    std::mt19937                          m_generator;
    std::uniform_real_distribution<float> m_uniform_dist;

public:
    WonderlandEngine();

    // Base uniform distribution [0, 1]. The foundation of all other methods.
    float get_uniform();

    // --- DISCRETE DISTRIBUTIONS ---

    // 1. Poisson Distribution (Knuth's algorithm)
    int get_poisson(float lambda);

    // 2. Geometric Distribution (Inverse transform sampling)
    // Returns the number of failures before the first success
    int get_geometric(float p);

    // 3. Random Permutation (Fisher-Yates shuffle)
    std::vector<int> get_permutation(int n);

    // 4. Binomial Distribution (Sum of n Bernoulli trials)
    int get_binomial(int n, float p);

    // --- CONTINUOUS DISTRIBUTIONS (Density-based) ---

    // 5. Beta Distribution (Acceptance-Rejection method)
    // Only implemented for alpha > 1 and beta > 1 for simplicity in this context
    float get_beta(float alpha, float beta_param);

    // 6. Normal Distribution (Box-Muller transform)
    float get_normal(float mu, float sigma);

    // 7. Exponential Distribution (Inverse transform sampling)
    float get_exponential(float lambda);

    // 8. Cauchy Distribution (Inverse transform sampling)
    float get_cauchy(float x0, float gamma);
};