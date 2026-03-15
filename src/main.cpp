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

    float value{0.f};
    int   current_piece_id{32};

    GameState  game_state;
    GameLogger logger;

    logger.AddLog("C'est parti !");

    quick_imgui::loop(
        "Chess",
        {
            .init = [&]() {
                pieces = pieces_gen(TILE_SIZE);
                assign_pos_pieces(pieces, TAB_POS);
            },
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
                        for (int i{0}; i < pieces.size(); i++)
                        {
                            ImGui::SetCursorPos(pieces[i].get_position());

                            ImGui::PushID(i);

                            if (pieces[i].is_playable() && pieces[i].show_piece(current_piece, game_state.is_finished, game_state.is_white_turn, pieces, TAB_POS))
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

                    if (game_state.is_finished)
                    {
                        ImGui::SetCursorScreenPos(boardStartPos);
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4{0.1f, 0.1f, 0.1f, 0.7f});
                        ImGui::BeginChild("End screen", ImVec2(TILE_SIZE * 8, TILE_SIZE * 8), false);

                        ImGui::SetWindowFontScale(1.5f);

                        ImVec2 buttonSize(180.f, 55.f);
                        float x_offset = (ImGui::GetWindowWidth() - buttonSize.x) * 0.5f;
                        float y_offset = (ImGui::GetWindowHeight() - buttonSize.y) * 0.5f;
                        ImGui::SetCursorPos(ImVec2(x_offset, y_offset));

                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.4f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.2f, 1.0f));

                        // RESTART
                        if (ImGui::Button("New Game", buttonSize))
                        {
                            game_state.is_white_turn = true;
                            game_state.is_finished   = false;

                            pieces = pieces_gen(TILE_SIZE);

                            assign_pos_pieces(pieces, TAB_POS);

                            current_piece = std::pair("", PIECE_STATUS::UNSELECTED);

                            current_piece_id = 32;

                            logger.AddLog("C'est parti !");
                        }

                        ImGui::PopStyleColor(3);
                        ImGui::SetWindowFontScale(1.0f);

                        ImGui::EndChild();
                        ImGui::PopStyleColor();
                    }

                    // --- Logs ---
                    logger.Draw(120.f);

                    ImGui::End();
                },
        }
    );
}