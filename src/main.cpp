#include <imgui.h>
#include <vector>
#include "chessboard.hpp"
// #include <imgui.h>
#include <iostream>
#include "chess_pieces.hpp"
#include "quick_imgui/quick_imgui.hpp"
#include "utils.hpp"
#include "game_state.hpp"
#include "logger.hpp"

std::vector<Piece> pieces{pieces_gen_v2()};

const float TILE_SIZE = 50.f;

const std::map<std::string, ImVec2> TAB_POS{generate_tab_position(TILE_SIZE)};

int main()
{
    assign_pos_pieces(pieces, TAB_POS);
    float value{0.f};

    for (int i{0}; i < 32; i++)
    {
        std::cout << pieces[i].get_name() << pieces[i].get_current_case() << '\n';
    }

    std::cout << "test: " << pieces[1].get_current_case() << '\n';

    GameState game_state;
    GameLogger logger;

    logger.AddLog("C'est parti !");
    
    quick_imgui::loop(
        "Chess",
        {
            .init = [&]() {},
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
                    if (game_state.is_white_turn) {
                        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "Blancs");
                        ImGui::SameLine();
                        ImGui::ColorButton("TurnW", ImVec4(1,1,1,1), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoPicker, ImVec2(15,15));
                    } else {
                        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), "Noirs");
                        ImGui::SameLine();
                        ImGui::ColorButton("TurnB", ImVec4(0,0,0,1), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoPicker, ImVec2(15,15));
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

                            if (pieces[i].show_piece()) 
                            {
                                if (pieces[i].is_white() == game_state.is_white_turn)
                                {
                                    logger.AddLog("Selection : " + pieces[i].get_name());
                                    
                                    // simulation fin tour pour tester alternance
                                    game_state.end_turn();
                                }
                                else
                                {
                                    logger.AddLog("[Erreur] Ce n'est pas votre tour !");
                                }
                            }
                            ImGui::PopID();
                        }
                    }
                    ImGui::EndChild();

                    // --- Logs ---
                    logger.Draw(120.f); 

                    ImGui::End();
                },
        }
    );
}