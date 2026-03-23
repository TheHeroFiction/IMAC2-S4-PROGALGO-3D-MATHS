#pragma once
#include <imgui.h>
#include <string>

struct GameState {
    bool is_white_turn = true; // Les blancs commencent toujours
    bool is_finished   = false;
    bool in_menu       = true;

    // Fonction pour changer le tour
    void end_turn()
    {
        is_white_turn = !is_white_turn;
    }

    // Fonction pour l'affichage
    std::string get_current_player_name() const
    {
        if (is_white_turn)
        {
            return "Blancs";
        }
        else
        {
            return "Noirs";
        }
    }

    void game_header() const
    {
        ImGui::Text("Tour actuel : ");
        ImGui::SameLine();
        if (is_finished)
        {
            ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "Game ended! ");
            ImGui::SameLine();
            if (is_white_turn)
            {
                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "Black won!");
            }
            else
            {
                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "White won!");
            }
        }
        else
        {
            if (is_white_turn)
            {
                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "Blancs");
                ImGui::SameLine();
                ImGui::ColorButton("TurnW", ImVec4(1, 1, 1, 1), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoPicker, ImVec2(15, 15));
            }
            else
            {
                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "Noirs");
                ImGui::SameLine();
                ImGui::ColorButton("TurnB", ImVec4(0, 0, 0, 1), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoPicker, ImVec2(15, 15));
            }
        }

        ImGui::Separator();
        ImGui::Spacing();
    }
};