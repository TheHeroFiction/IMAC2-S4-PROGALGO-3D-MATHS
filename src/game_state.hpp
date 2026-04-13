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

    // Echoes of Wonderland
    WonderlandLore::Event current_event = WonderlandLore::Event::NONE;
    WonderlandLore::Quote current_quote;

    void trigger_echo(WonderlandLore::Event event)
    {
        current_event = event;
        current_quote = WonderlandLore::get_quote(event);
    }
    // -----------------------

    void end_turn()
    {
        is_white_turn = !is_white_turn;

        if (is_wonderland_mode)
        {
            float roll = alice_engine.get_uniform();

            if (roll < 0.33f)
            {
                trigger_echo(WonderlandLore::Event::MAGIC_MUSHROOM);
            }
            else if (roll < 0.66f)
            {
                trigger_echo(WonderlandLore::Event::WHITE_RABBIT);
            }
            else
            {
                trigger_echo(WonderlandLore::Event::NONE);
            }
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

        ImGui::Separator();
        ImGui::Spacing();
    }
};