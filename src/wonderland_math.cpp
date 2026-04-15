/* =====================================================================
 * WONDERLAND ENGINE
 * Math & Probability Generation
 * Note: This module was structured with the assistance of an AI
 * to correctly implement the probability distributions required.
 * ===================================================================== */

#include "wonderland_math.hpp"
#include <algorithm> // for std::swap, std::max, std::min
#include <cmath>
#include <numbers>

WonderlandEngine::WonderlandEngine()
    : m_generator(std::random_device{}()), m_uniform_dist(0.0f, 1.0f) {}

float WonderlandEngine::get_uniform()
{
    return m_uniform_dist(m_generator);
}

// --- CONTINUOUS DISTRIBUTIONS ---

float WonderlandEngine::get_normal(float mu, float sigma)
{
    float u1 = get_uniform();
    float u2 = get_uniform();

    // Prevent log(0) using std::max
    u1 = std::max(1e-6f, u1);

    // Box-Muller transform using float versions of math functions
    float z0 = std::sqrt(-2.0f * std::log(u1)) * std::cos(2.0f * std::numbers::pi_v<float> * u2);

    return mu + z0 * sigma;
}

float WonderlandEngine::get_exponential(float lambda)
{
    float u = get_uniform();
    // Inverse transform
    return -std::log(1.0f - u) / lambda;
}

float WonderlandEngine::get_cauchy(float x0, float gamma)
{
    float u = get_uniform();
    // Inverse transform
    return x0 + gamma * std::tan(std::numbers::pi_v<float> * (u - 0.5f));
}

float WonderlandEngine::get_beta(float alpha, float beta_param)
{
    // Acceptance-Rejection method
    if (alpha <= 1.0f || beta_param <= 1.0f)
    {
        return get_uniform();
    }

    float c_max = std::pow((alpha - 1.0f) / (alpha + beta_param - 2.0f), alpha - 1.0f) * std::pow((beta_param - 1.0f) / (alpha + beta_param - 2.0f), beta_param - 1.0f);

    while (true)
    {
        float x   = get_uniform();
        float y   = get_uniform() * c_max;
        float f_x = std::pow(x, alpha - 1.0f) * std::pow(1.0f - x, beta_param - 1.0f);

        if (y < f_x)
        {
            return x;
        }
    }
}

// --- DISCRETE DISTRIBUTIONS ---

int WonderlandEngine::get_poisson(float lambda)
{
    // Knuth's algorithm (rewritten to avoid do-while loop)
    float L = std::exp(-lambda);
    int   k = 0;
    float p = 1.0f;

    while (true)
    {
        k++;
        p *= get_uniform();
        if (p <= L)
        {
            break;
        }
    }
    return k - 1;
}

int WonderlandEngine::get_geometric(float p)
{
    if (p <= 0.0f || p > 1.0f)
        return 0;
    float u = get_uniform();
    // Inverse transform
    return static_cast<int>(std::floor(std::log(u) / std::log(1.0f - p)));
}

int WonderlandEngine::get_binomial(int n, float p)
{
    int successes = 0;
    for (int i = 0; i < n; ++i)
    {
        if (get_uniform() < p)
        {
            successes++;
        }
    }
    return successes;
}

std::vector<int> WonderlandEngine::get_permutation(int n)
{
    std::vector<int> indices(n);
    for (int i = 0; i < n; ++i)
    {
        indices[i] = i;
    }

    // Fisher-Yates shuffle
    for (int i = n - 1; i > 0; --i)
    {
        int j = static_cast<int>(get_uniform() * static_cast<float>(i + 1));
        j     = std::min(i, j); // Safety clamp instead of manual if
        std::swap(indices[i], indices[j]);
    }
    return indices;
}