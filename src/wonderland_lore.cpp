#include "wonderland_lore.hpp"

namespace WonderlandLore {

Quote get_quote(Event event)
{
    switch (event)
    {
    case Event::MAD_HATTER:
        return {
            .source      = "Alice's Adventures in Wonderland, Chapter 7 (\"A Mad Tea-Party\")",
            .text        = "\"I want a clean cup, let's all move one place on!\"",
            .explanation = "The Mad Hatter's Tea Party : The order of the major pieces is thrown into total chaos using a random permutation (Fisher-Yates)."
        };

    case Event::MAGIC_MUSHROOM:
        return {
            .source      = "Alice's Adventures in Wonderland, Chapter 5 (\"Advice from a Caterpillar\")",
            .text        = "\"One side will make you grow taller, and the other side will make you grow shorter.\"",
            .explanation = "The Magic Mushroom: Piece visual scale is distorted using a Beta distribution (ensuring a bounded multiplier between 0 and 1)."
        };

    case Event::WHITE_RABBIT:
        return {
            .source      = "Alice's Adventures in Wonderland, Chapter 1 (\"Down the Rabbit-Hole\")",
            .text        = "\"Oh my ears and whiskers, how late it's getting!\"",
            .explanation = "The White Rabbit's Anxiety: Pieces jitter on their tiles. This visual noise is generated with a Normal (Gaussian) distribution."
        };

    case Event::RED_QUEEN:
        return {
            .source      = "Through the Looking-Glass, Chapter 2 (\"The Garden of Live Flowers\")",
            .text        = "\"Now, here, you see, it takes all the running you can do, to keep in the same place.\"",
            .explanation = "The Red Queen's Race: A piece may stall in place. The number of attempts required before it can be selected follows a Geometric distribution."
        };

    case Event::CHESHIRE_CAT:
        return {
            .source      = "Alice's Adventures in Wonderland, Chapter 6 (\"Pig and Pepper\")",
            .text        = "\"A grin without a cat!\"",
            .explanation = "The Cheshire Smile: Ghostly appearances block some tiles. The count of these rare events follows a Poisson distribution."
        };

    case Event::CARD_GARDENERS:
        return {
            .source      = "Alice's Adventures in Wonderland, Chapter 8 (\"The Queen's Croquet-Ground\")",
            .text        = "\"Look out now, Five! Don't go splashing paint over me like that!\"",
            .explanation = "The Card Gardeners: A magical brush strikes the board. The number of pieces affected by paint follows a Binomial distribution."
        };

    case Event::ENDLESS_FALL:
        return {
            .source      = "Alice's Adventures in Wonderland, Chapter 1 (\"Down the Rabbit-Hole\")",
            .text        = "\"Down, down, down. Would the fall never come to an end!\"",
            .explanation = "The Endless Fall: Thinking time varies unpredictably according to an Exponential distribution (memoryless waiting time)."
        };

    case Event::LOOKING_GLASS:
        return {
            .source      = "Through the Looking-Glass, Chapter 2 (\"The Garden of Live Flowers\")",
            .text        = "\"It's a great huge game of chess that's being played-all over the world!\"",
            .explanation = "The Looking Glass: Space bends. Sometimes, a move experiences an extreme drift to another tile, driven by the heavy tails of the Cauchy distribution."
        };

    default:
        return {.source = "", .text = "", .explanation = ""};
    }
}

} // namespace WonderlandLore