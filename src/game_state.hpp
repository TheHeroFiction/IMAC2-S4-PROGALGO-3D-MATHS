#pragma once
#include <imgui.h>
#include <string>
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
    float turn_timer = 0.0f;
    float current_time_limit = 15.0f;

    std::vector<std::string> trapped_tiles;

    void reset_timer() {
        float raw_time = alice_engine.get_exponential(0.066f);
        current_time_limit = std::max(5.0f, std::min(30.0f, raw_time));
        turn_timer = 0.0f;
    }
    // --- [END AI-GENERATED] ---

    void trigger_echo(WonderlandLore::Event event) {
        current_echo  = event;
        current_quote = WonderlandLore::get_quote(event);
    }
    // -----------------------

    void end_turn()
    {
        is_white_turn = !is_white_turn;

        if (is_wonderland_mode) 
        {
            float roll = alice_engine.get_uniform(); 
            WonderlandLore::Event new_weather = WonderlandLore::Event::NONE;

            if (roll < 0.20f) {
                new_weather = WonderlandLore::Event::MAGIC_MUSHROOM;
            } 
            else if (roll < 0.40f) {
                new_weather = WonderlandLore::Event::WHITE_RABBIT;
            }
            else if (roll < 0.60f) {
                new_weather = WonderlandLore::Event::CARD_GARDENERS;
            }
            else if (roll < 0.80f) {
                new_weather = WonderlandLore::Event::CHESHIRE_CAT;
            }
            
            current_weather = new_weather;
            trigger_echo(new_weather); 
            
            reset_timer(); 
        }
    }

    // --- NAME THE CURRENT PLAYER IN HEADER ---
    std::string get_current_player_name() const
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

    void game_header() const
    {
        ImGui::Text("Current turn : ");
        ImGui::SameLine();
        if (is_finished)
        {
            ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "Game ended! ");
            ImGui::SameLine();
            if (is_white_turn)
            {
                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "Black pieces won!");
            }
            else
            {
                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "White pieces won!");
            }
        }
        else
        {
            if (is_white_turn)
            {
                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "White pieces");
                ImGui::SameLine();
                ImGui::ColorButton("TurnW", ImVec4(1, 1, 1, 1), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoPicker, ImVec2(15, 15));
            }
            else
            {
                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "Black pieces");
                ImGui::SameLine();
                ImGui::ColorButton("TurnB", ImVec4(0, 0, 0, 1), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoPicker, ImVec2(15, 15));
            }
        }

        // --- [BEGIN AI-GENERATED] TIMER UI ---
        if (is_wonderland_mode && !is_finished) 
        {
            ImGui::SameLine();
            float time_left = std::max(0.0f, current_time_limit - turn_timer);
            
            ImVec4 timer_color = (time_left <= 5.0f) ? ImVec4(1.0f, 0.2f, 0.2f, 1.0f) : ImVec4(1.0f, 0.8f, 0.2f, 1.0f);
            
            ImGui::TextColored(timer_color, " | Time left: %.1fs", time_left);
        }
        // --- [END AI-GENERATED] ---

        ImGui::Separator();
        ImGui::Spacing();
    }
};