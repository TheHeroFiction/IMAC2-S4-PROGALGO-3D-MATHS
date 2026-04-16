#pragma once
#include <array>
#include <string>
#include <vector>
#include "wonderland_lore.hpp"
#include "wonderland_math.hpp"

struct GameState {
    bool is_white_turn = true;
    bool is_finished   = false;
    bool in_menu       = true;

    // --- WONDERLAND MODE ---
    bool             is_wonderland_mode = false;
    WonderlandEngine alice_engine;

    // --- WONDERLAND STATE ---
    WonderlandLore::Event current_weather = WonderlandLore::Event::NONE;
    WonderlandLore::Event current_echo    = WonderlandLore::Event::NONE;
    WonderlandLore::Quote current_quote;

    // --- [BEGIN AI-GENERATED] ENDLESS FALL VARIABLES ---
    float turn_timer         = 0.0f;
    float current_time_limit = 15.0f;

    std::vector<std::string> trapped_tiles;

    // --- STATISTICS TRACKING ---
    std::vector<float> exp_history;
    std::vector<int>   poisson_history;
    std::vector<int>   geometric_history;
    std::vector<float> cauchy_history;
    std::vector<int>   binomial_history;
    std::vector<float> normal_history;
    std::vector<float> beta_history;

    // Buckets: 0=Beta, 1=Normal, 2=Binomial, 3=Poisson, 4=Clear(NONE)
    std::array<int, 5> weather_distribution{};

    void reset_wonderland_stats();

    void reset_timer();
    // --- [END AI-GENERATED] ---

    void trigger_echo(WonderlandLore::Event event);
    // -----------------------

    void end_turn();

    // --- NAME THE CURRENT PLAYER IN HEADER ---
    std::string get_current_player_name() const;
};