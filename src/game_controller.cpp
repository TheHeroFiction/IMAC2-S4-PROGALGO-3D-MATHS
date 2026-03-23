#include "game_controller.hpp"

std::vector<std::pair<std::string, Behaviour>> titles_for_promotions{{"Rook", Behaviour::Rook}, {"Knight", Behaviour::Knight}, {"Bishop", Behaviour::Bishop}, {"Queen", Behaviour::Queen}};

void show_mode_selection(ImVec2 boardStartPos, GameState& game_state, GameLogger& logger, float TILE_SIZE, std::vector<Piece>& pieces, const std::map<std::string, ImVec2>& TAB_POS, std::pair<std::string, PIECE_STATUS>& current_piece, int current_piece_id)
{
    ImGui::SetCursorScreenPos(boardStartPos);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4{0.1f, 0.1f, 0.1f, 0.7f});
    ImGui::BeginChild("Mode Selection", ImVec2(TILE_SIZE * 8, TILE_SIZE * 8), false);

    ImGui::SetWindowFontScale(1.3f);

    ImVec2 buttonSize(200.f, 50.f);
    float  spacing = 15.f;

    float total_height = (buttonSize.y * 3) + (spacing * 2);

    float x_offset = (ImGui::GetWindowWidth() - buttonSize.x) * 0.5f;
    float y_offset = (ImGui::GetWindowHeight() - total_height) * 0.5f;

    ImGui::SetCursorPos(ImVec2(x_offset, y_offset));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.2f, 1.0f));

    // Mode Normal (ou Restart)
    std::string btn_label = game_state.is_finished ? "Rejouer Normal" : "Mode normal";
    if (ImGui::Button(btn_label.c_str(), buttonSize))
    {
        game_state.in_menu       = false;
        game_state.is_finished   = false;
        game_state.is_white_turn = true;

        pieces = pieces_gen(TILE_SIZE);
        assign_pos_pieces(pieces, TAB_POS);

        current_piece    = std::pair("", PIECE_STATUS::UNSELECTED);
        current_piece_id = 32;

        logger.Clear();
        logger.AddLog("C'est parti en mode normal !");
    }

    ImGui::SetCursorPosX(x_offset);
    ImGui::Dummy(ImVec2(0.f, spacing));
    ImGui::SetCursorPosX(x_offset);

    // Mode 3D
    if (ImGui::Button("Mode 3D", buttonSize))
    {
        logger.AddLog("Le mode 3D sera bientôt disponible !");
    }

    ImGui::SetCursorPosX(x_offset);
    ImGui::Dummy(ImVec2(0.f, spacing));
    ImGui::SetCursorPosX(x_offset);

    // Mode Aléatoire
    if (ImGui::Button("Mode aléatoire", buttonSize))
    {
        logger.AddLog("Le mode aléatoire sera bientot disponible !");
    }

    ImGui::PopStyleColor(3);
    ImGui::SetWindowFontScale(1.0f);

    ImGui::EndChild();
    ImGui::PopStyleColor();
};

void promotion_screen(bool& to_be_promoted, std::vector<Piece>& pieces)
{
    if (to_be_promoted)
    {
        ImGui::OpenPopup("Promote");

        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Promote", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Choose the promotion you want !");
            ImGui::Separator();

            for (int i{0}; i < titles_for_promotions.size(); i++)
            {
                if (ImGui::Button(titles_for_promotions[i].first.c_str(), ImVec2(120, 0)))
                {
                    to_be_promoted = false;
                    pieces[0].set_behaviour(titles_for_promotions[i].second);
                    pieces[0].set_texture_id(give_texture_id(titles_for_promotions[i].second, pieces[0].is_white()));
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
            }
            ImGui::EndPopup();
        }
    }
}