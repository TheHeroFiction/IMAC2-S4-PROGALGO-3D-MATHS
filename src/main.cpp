#include <utility>
#include <vector>
#include "chessboard.hpp"
#include "game_controller.hpp"
#include "quick_imgui/quick_imgui.hpp"
#include "utils.hpp"

const float TILE_SIZE = 70.f;

int main()
{
    std::vector<Piece>                   pieces;
    const std::map<std::string, ImVec2>  TAB_POS{generate_tab_position(TILE_SIZE)};
    std::pair<std::string, PIECE_STATUS> current_piece{"", PIECE_STATUS::UNSELECTED};

    int  current_piece_id{32};
    bool to_be_promoted{false};

    GameState  game_state;
    GameLogger logger;

    quick_imgui::loop(
        "Chess",
        {
            .init = [&]() {
                pieces = pieces_gen(TILE_SIZE);
                assign_pos_pieces(pieces, TAB_POS); },
            .loop =
                [&]() {
                    // ImGui::ShowDemoWindow();

                    ImGui::Begin("Chess Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

                    // --- HEADER ---
                    game_state.game_header();

                    // --- CHESSBOARD & PIECES ---
                    ImVec2 boardStartPos = ImGui::GetCursorScreenPos();

                    draw_board(TILE_SIZE);

                    // --- [BEGIN AI-GENERATED] DRAW CHESHIRE TRAPS ---
                    if (game_state.is_wonderland_mode && !game_state.trapped_tiles.empty()) 
                    {
                        ImDrawList* draw_list = ImGui::GetWindowDrawList();
                        for (const std::string& trap_tile : game_state.trapped_tiles) 
                        {
                            if (TAB_POS.find(trap_tile) != TAB_POS.end())
                            {
                                ImVec2 pos = TAB_POS.at(trap_tile);

                                ImVec2 p_min = ImVec2(boardStartPos.x + pos.x, boardStartPos.y + pos.y);
                                ImVec2 p_max = ImVec2(p_min.x + TILE_SIZE, p_min.y + TILE_SIZE);
                                draw_list->AddRectFilled(p_min, p_max, IM_COL32(150, 50, 200, 255));
                            }
                        }
                    }
                    // --- [END AI-GENERATED] ---

                    ImGui::SetCursorScreenPos(boardStartPos);

                    ImGui::BeginChild("PiecesLayer", ImVec2(TILE_SIZE * 8, TILE_SIZE * 8), false, ImGuiWindowFlags_NoBackground);
                    {
                        promotion_screen(to_be_promoted, pieces);

                        draw_pieces(game_state, logger, to_be_promoted, pieces, TAB_POS, current_piece, current_piece_id);
                    }
                    ImGui::EndChild();

                    if (game_state.in_menu || game_state.is_finished)
                    {
                        game_menu(boardStartPos, game_state, logger, TILE_SIZE, pieces, TAB_POS, current_piece, current_piece_id);
                    }

                    // --- Logs ---
                    logger.Draw(120.f);

                    // --- [BEGIN AI-GENERATED] ENDLESS FALL TICK ---
                    if (game_state.is_wonderland_mode && !game_state.is_finished && !game_state.in_menu) 
                    {
                        game_state.turn_timer += ImGui::GetIO().DeltaTime;

                        if (game_state.turn_timer >= game_state.current_time_limit) 
                        {
                            logger.AddLog("[Endless Fall] Time's up! Turn skipped.");
                            current_piece = {"", PIECE_STATUS::UNSELECTED};
                            current_piece_id = 32;

                            game_state.end_turn();

                            apply_weather_effects(game_state, pieces);

                            game_state.trigger_echo(WonderlandLore::Event::ENDLESS_FALL);
                        }
                    }
                    // --- [END AI-GENERATED] ---

                    // --- WINDOW: ECHOES OF WONDERLAND ---
                    if (game_state.is_wonderland_mode && game_state.current_echo != WonderlandLore::Event::NONE)
                    {
                        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 320, 20), ImGuiCond_FirstUseEver);
                        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_Always);

                        ImGui::Begin("Echoes of Wonderland", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.3f, 1.0f));
                        ImGui::TextWrapped("%s", game_state.current_quote.source.c_str());
                        ImGui::PopStyleColor();
                        ImGui::Separator();

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.8f, 0.8f, 1.0f));
                        ImGui::TextWrapped("%s", game_state.current_quote.text.c_str());
                        ImGui::PopStyleColor();

                        ImGui::Separator();
                        ImGui::TextWrapped("Explanation:");
                        ImGui::TextWrapped("%s", game_state.current_quote.explanation.c_str());

                        ImGui::End();
                    }

                    ImGui::End();
                },
        }
    );
}
