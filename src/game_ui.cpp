#include "game_ui.hpp"
#include <imgui.h>
#include <algorithm>

void draw_game_header(const GameState& game_state)
{
    ImGui::Text("Current turn : ");
    ImGui::SameLine();
    if (game_state.is_finished)
    {
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "Game ended! ");
        ImGui::SameLine();
        if (game_state.is_white_turn)
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
        if (game_state.is_white_turn)
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

    if (game_state.is_wonderland_mode && !game_state.is_finished)
    {
        ImGui::SameLine();
        float time_left = std::max(0.0f, game_state.current_time_limit - game_state.turn_timer);

        ImVec4 timer_color = (time_left <= 5.0f) ? ImVec4(1.0f, 0.2f, 0.2f, 1.0f) : ImVec4(1.0f, 0.8f, 0.2f, 1.0f);

        ImGui::TextColored(timer_color, " | Time left: %.1fs", time_left);
    }

    ImGui::Separator();
    ImGui::Spacing();
}
