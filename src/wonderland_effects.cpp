#include "wonderland_effects.hpp"
#include <algorithm>

namespace {

void reset_visual_state(std::vector<Piece>& all_pieces)
{
    for (auto& p : all_pieces)
    {
        p.set_painted_red(false);
        p.set_magic_scale(1.0f);
    }
}

void apply_magic_mushroom(GameState& game_state, std::vector<Piece>& all_pieces)
{
    if (game_state.current_weather != WonderlandLore::Event::MAGIC_MUSHROOM)
    {
        return;
    }

    for (auto& p : all_pieces)
    {
        float beta_val = game_state.alice_engine.get_beta(2.0f, 2.0f);
        game_state.beta_history.push_back(beta_val);

        p.set_magic_scale(0.5f + beta_val);
    }
}

void apply_card_gardeners(GameState& game_state, std::vector<Piece>& all_pieces)
{
    if (game_state.current_weather != WonderlandLore::Event::CARD_GARDENERS)
    {
        return;
    }

    std::vector<int> white_indices;
    const int        piece_count = static_cast<int>(all_pieces.size());

    for (int i = 0; i < piece_count; ++i)
    {
        if (all_pieces[i].is_playable() && all_pieces[i].is_white())
        {
            white_indices.push_back(i);
        }
    }

    int n = static_cast<int>(white_indices.size());
    if (n <= 0)
    {
        return;
    }

    int painted_count = game_state.alice_engine.get_binomial(n, 0.3f);
    game_state.binomial_history.push_back(painted_count);
    std::vector<int> shuffled = game_state.alice_engine.get_permutation(n);

    for (int i = 0; i < painted_count; ++i)
    {
        all_pieces[white_indices[shuffled[i]]].set_painted_red(true);
    }
}

void apply_cheshire_cat(GameState& game_state, std::vector<Piece>& all_pieces)
{
    if (game_state.current_weather != WonderlandLore::Event::CHESHIRE_CAT)
    {
        return;
    }

    std::vector<std::string> all_board_tiles;
    for (int x = 0; x < 8; ++x)
    {
        for (int y = 1; y <= 8; ++y)
        {
            char file = static_cast<char>('a' + x);
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
        {
            empty_tiles.push_back(tile);
        }
    }

    int raw_trap_count = game_state.alice_engine.get_poisson(2.0f);

    // --- STATISTICS TRACKING ---
    game_state.poisson_history.push_back(raw_trap_count);
    // ----------------------

    int trap_count = std::min(raw_trap_count, static_cast<int>(empty_tiles.size()));

    if (trap_count > 0)
    {
        std::vector<int> shuffled = game_state.alice_engine.get_permutation(static_cast<int>(empty_tiles.size()));
        for (int i = 0; i < trap_count; ++i)
        {
            game_state.trapped_tiles.push_back(empty_tiles[shuffled[i]]);
        }
    }
    else
    {
        game_state.current_echo = WonderlandLore::Event::NONE;
    }
}

} // namespace

// --- [BEGIN AI-GENERATED] WONDERLAND WEATHER UPDATES ---
void apply_weather_effects(GameState& game_state, std::vector<Piece>& all_pieces)
{
    reset_visual_state(all_pieces);

    game_state.trapped_tiles.clear();

    // --- MAGIC MUSHROOM (BETA LAW) ---
    apply_magic_mushroom(game_state, all_pieces);

    // --- CARD GARDENERS (BINOMIAL LAW) ---
    apply_card_gardeners(game_state, all_pieces);

    // --- CHESHIRE CAT (POISSON LAW) ---
    apply_cheshire_cat(game_state, all_pieces);
}
// --- [END AI-GENERATED] ---