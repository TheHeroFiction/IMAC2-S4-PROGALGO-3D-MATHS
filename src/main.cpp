#include <imgui.h>
#include <vector>
#include "chessboard.hpp"
// #include <imgui.h>
#include <iostream>
#include <utility>
#include "chess_pieces.hpp"
#include "game_state.hpp"
#include "logger.hpp"
#include "quick_imgui/quick_imgui.hpp"
#include "utils.hpp"

const float TILE_SIZE = 50.f;

int main()
{
    std::vector<Piece>                   pieces;
    const std::map<std::string, ImVec2>  TAB_POS{generate_tab_position(TILE_SIZE)};
    std::pair<std::string, PIECE_STATUS> current_piece{"", PIECE_STATUS::UNSELECTED};

    std::vector<std::pair<std::string, Behaviour>> titles_for_promotions{{"Rook", Behaviour::Rook}, {"Knight", Behaviour::Knight}, {"Bishop", Behaviour::Bishop}, {"Queen", Behaviour::Queen}};

    float value{0.f};
    int   current_piece_id{32};
    bool  to_be_promoted{false};

    GameState  game_state;
    GameLogger logger;

    auto show_mode_selection = [&](ImVec2 boardStartPos) {
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

    quick_imgui::loop(
        "Chess",
        {
            .init = [&]() {
                pieces = pieces_gen(TILE_SIZE);
                assign_pos_pieces(pieces, TAB_POS); },
            .loop =
                [&]() {
                    ImGui::ShowDemoWindow(); // This opens a window which shows tons of examples of what you can do with ImGui. You should check it out! Also, you can use the "Item Picker" in the top menu of that demo window: then click on any widget and it will show you the corresponding code directly in your IDE!

                    // ImGui::Begin("Example");

                    // ImGui::SliderFloat("My Value", &value, 0.f, 3.f);

                    // if (ImGui::Button("1", ImVec2{50.f, 50.f}))
                    //     std::cout << "Clicked button 1\n";
                    // ImGui::SameLine(); // Draw the next ImGui widget on the same line as the previous one. Otherwise it would be below it

                    // ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.f, 0.f, 0.f, 1.f}); // Changes the color of all buttons until we call ImGui::PopStyleColor(). There is also ImGuiCol_ButtonActive and ImGuiCol_ButtonHovered

                    // ImGui::PushID(2); // When some ImGui items have the same label (for exemple the next two buttons are labeled "Yo") ImGui needs you to specify an ID so that it can distinguish them. It can be an int, a pointer, a string, etc.
                    //                   // You will definitely run into this when you create a button for each of your chess pieces, so remember to give them an ID!
                    // if (ImGui::Button("Yo", ImVec2{50.f, 50.f}))
                    //     std::cout << "Clicked button 2\n";
                    // ImGui::PopID(); // Then pop the id you pushed after you created the widget

                    // ImGui::SameLine();
                    // ImGui::PushID(3);
                    // if (ImGui::Button("Yo", ImVec2{50.f, 50.f}))
                    //     std::cout << "Clicked button 3\n";
                    // ImGui::PopID();

                    // ImGui::PopStyleColor();

                    ImGui::Begin("Chess Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

                    // --- Header ---
                    ImGui::Text("Tour actuel : ");
                    ImGui::SameLine();
                    if (game_state.is_finished)
                    {
                        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "Game ended! ");
                        ImGui::SameLine();
                        if (game_state.is_white_turn)
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
                        if (game_state.is_white_turn)
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

                    // --- Le Plateau et les Pièces ---
                    ImVec2 boardStartPos = ImGui::GetCursorScreenPos();

                    draw_board(TILE_SIZE);

                    ImGui::SetCursorScreenPos(boardStartPos);

                    ImGui::BeginChild("PiecesLayer", ImVec2(TILE_SIZE * 8, TILE_SIZE * 8), false, ImGuiWindowFlags_NoBackground);
                    {
                        if (to_be_promoted)
                        {
                            ImGui::OpenPopup("Promote");

                            // Always center this window when appearing
                            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                            if (ImGui::BeginPopupModal("Promote", NULL, ImGuiWindowFlags_AlwaysAutoResize))
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
                        for (int i{0}; i < pieces.size(); i++)
                        {
                            ImGui::SetCursorPos(pieces[i].get_position());

                            ImGui::PushID(i);

                            if (pieces[i].is_playable() && pieces[i].show_piece(current_piece, game_state.is_finished, game_state.is_white_turn, pieces, TAB_POS, to_be_promoted))
                            {
                                if (pieces[i].is_white() == game_state.is_white_turn)
                                {
                                    logger.AddLog("Selection : " + pieces[i].get_name());

                                    current_piece    = {pieces[i].get_name(), PIECE_STATUS::SELECTED};
                                    current_piece_id = i;
                                }
                                else
                                {
                                    logger.AddLog("[Erreur] Ce n'est pas votre tour !");
                                }
                            }
                            ImGui::PopID();
                        }
                        // Put the selected piece in the first place in order to draw it first
                        if (current_piece_id != 32 && pieces[0].get_name() != pieces[current_piece_id].get_name())
                        {
                            // problem with where pieces have been eaten in start position
                            Piece temp{pieces[current_piece_id]};
                            pieces[current_piece_id] = pieces[0];
                            pieces[0]                = temp;
                            current_piece_id         = 32;
                        }
                    }
                    ImGui::EndChild();

                    if (game_state.in_menu || game_state.is_finished)
                    {
                        show_mode_selection(boardStartPos);
                    }

                    // --- Logs ---
                    logger.Draw(120.f);

                    ImGui::End();
                },
        }
    );
}
