#include "wonderland_effects.hpp"
#include <algorithm>

// --- [BEGIN AI-GENERATED] WONDERLAND WEATHER UPDATES ---
void apply_weather_effects(GameState& game_state, std::vector<Piece>& all_pieces)
{
    for (auto& p : all_pieces)
    {
        p.set_painted_red(false);
    }

    game_state.trapped_tiles.clear();

    // --- MAGIC MUSHROOM (BETA LAW) ---
    if (game_state.current_weather == WonderlandLore::Event::MAGIC_MUSHROOM)
    {
        for (auto& p : all_pieces)
        {
            p.set_magic_scale(0.5f + game_state.alice_engine.get_beta(2.0f, 2.0f));
        }
    }
    else
    {
        for (auto& p : all_pieces)
        {
            p.set_magic_scale(1.0f);
        }
    }

    // --- CARD GARDENERS (BINOMIAL LAW) ---
    if (game_state.current_weather == WonderlandLore::Event::CARD_GARDENERS)
    {
        std::vector<int> white_indices;
        for (int i = 0; i < all_pieces.size(); ++i)
        {
            if (all_pieces[i].is_playable() && all_pieces[i].is_white())
            {
                white_indices.push_back(i);
            }
        }

        int n = white_indices.size();
        if (n > 0)
        {
            int              painted_count = game_state.alice_engine.get_binomial(n, 0.3f);
            std::vector<int> shuffled      = game_state.alice_engine.get_permutation(n);

            for (int i = 0; i < painted_count; ++i)
            {
                all_pieces[white_indices[shuffled[i]]].set_painted_red(true);
            }
        }
    }

    // --- CHESHIRE CAT (POISSON LAW) ---
    if (game_state.current_weather == WonderlandLore::Event::CHESHIRE_CAT)
    {
        std::vector<std::string> all_board_tiles;
        for (int x = 0; x < 8; ++x)
        {
            for (int y = 1; y <= 8; ++y)
            {
                char file = 'a' + x;
                all_board_tiles.push_back(std::string(1, file) + std::to_string(y));
            }
        }

        std::vector<std::string> empty_tiles;
        for (const std::string& tile : all_board_tiles)
        {
            bool is_occupied = false;
            for (const auto& p : all_pieces)
            {
                if (p.is_playable() && p.get_current_tile() == tile)
                {
                    is_occupied = true;
                    break;
                }
            }
            if (!is_occupied)
                empty_tiles.push_back(tile);
        }

        int trap_count = game_state.alice_engine.get_poisson(2.0f);

        trap_count = std::max(1, trap_count);

        trap_count = std::min(trap_count, static_cast<int>(empty_tiles.size()));

        if (trap_count > 0)
        {
            std::vector<int> shuffled = game_state.alice_engine.get_permutation(empty_tiles.size());
            for (int i = 0; i < trap_count; ++i)
            {
                game_state.trapped_tiles.push_back(empty_tiles[shuffled[i]]);
            }
        }
    }
}
// --- [END AI-GENERATED] ---