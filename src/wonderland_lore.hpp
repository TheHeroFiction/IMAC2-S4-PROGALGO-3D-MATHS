/* =====================================================================
 * WONDERLAND ENGINE
 * Math & Probability Generation
 * Note: This module was structured with the assistance of an AI
 * to correctly implement the probability distributions required.
 * ===================================================================== */

#pragma once
#include <cstdint> // For std::uint8_t
#include <string>

namespace WonderlandLore {

// List of the 8 magical events (using uint8_t to save memory as suggested by linter)
enum class Event : std::uint8_t {
    NONE = 0,
    CHESHIRE_CAT,   // Poisson
    RED_QUEEN,      // Geometric
    MAD_HATTER,     // Random Permutation
    CARD_GARDENERS, // Binomial
    MAGIC_MUSHROOM, // Beta
    WHITE_RABBIT,   // Normal
    ENDLESS_FALL,   // Exponential
    LOOKING_GLASS   // Cauchy
};

struct Quote {
    std::string source;
    std::string text;
    std::string explanation;
};

Quote get_quote(Event event);

} // namespace WonderlandLore