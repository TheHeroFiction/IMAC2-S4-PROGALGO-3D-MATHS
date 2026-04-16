#include "game_state.hpp"
#include <imgui.h>
#include <algorithm>

void GameState::reset_wonderland_stats()
{
    exp_history.clear();
    poisson_history.clear();
    geometric_history.clear();
    cauchy_history.clear();
    binomial_history.clear();
    normal_history.clear();
    beta_history.clear();
    weather_distribution.fill(0);
}

void GameState::reset_timer()
{
    float raw_time = alice_engine.get_exponential(0.066f);

    exp_history.push_back(raw_time);

    current_time_limit = std::max(5.0f, std::min(30.0f, raw_time));
    turn_timer         = 0.0f;
}

void GameState::trigger_echo(WonderlandLore::Event event)
{
    current_echo  = event;
    current_quote = WonderlandLore::get_quote(event);
}

void GameState::end_turn()
{
    is_white_turn = !is_white_turn;

    if (is_wonderland_mode)
    {
        float                 roll        = alice_engine.get_uniform();
        WonderlandLore::Event new_weather = WonderlandLore::Event::NONE;

        if (roll < 0.20f)
        {
            new_weather = WonderlandLore::Event::MAGIC_MUSHROOM;
        }
        else if (roll < 0.40f)
        {
            new_weather = WonderlandLore::Event::WHITE_RABBIT;
        }
        else if (roll < 0.60f)
        {
            new_weather = WonderlandLore::Event::CARD_GARDENERS;
        }
        else if (roll < 0.80f)
        {
            new_weather = WonderlandLore::Event::CHESHIRE_CAT;
        }

        if (roll < 0.20f)
        {
            weather_distribution[0]++;
        }
        else if (roll < 0.40f)
        {
            weather_distribution[1]++;
        }
        else if (roll < 0.60f)
        {
            weather_distribution[2]++;
        }
        else if (roll < 0.80f)
        {
            weather_distribution[3]++;
        }
        else
        {
            weather_distribution[4]++;
        }

        current_weather = new_weather;
        trigger_echo(new_weather);

        reset_timer();
    }
}

std::string GameState::get_current_player_name() const
{
    if (is_white_turn)
    {
        return "White pieces";
    }
    else
    {
        return "Black pieces";
    }
}
