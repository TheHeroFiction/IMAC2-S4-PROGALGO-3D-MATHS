#pragma once
#include <imgui.h>
#include <string>

struct GameState {
    bool is_white_turn = true;
    bool is_finished   = false;
    bool in_menu       = true;

    void end_turn()
    {
        is_white_turn = !is_white_turn;
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