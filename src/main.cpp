#include <utility>
#include <vector>
#include "chessboard.hpp"
#include "game_controller.hpp"
#include "quick_imgui/quick_imgui.hpp"
#include "utils.hpp"
#include "wonderland_runtime.hpp"
#include "wonderland_stats.hpp"

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

                    ImGui::SetNextWindowPos(ImVec2(20.0f, 64.0f), ImGuiCond_FirstUseEver);
                    ImGui::Begin("Chess Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

                    // --- HEADER ---
                    game_state.game_header();

                    // --- CHESSBOARD & PIECES ---
                    ImVec2 boardStartPos = ImGui::GetCursorScreenPos();

                    draw_board(TILE_SIZE);

                    // --- [BEGIN AI-GENERATED] DRAW CHESHIRE TRAPS ---
                    draw_wonderland_traps(game_state, TAB_POS, boardStartPos, TILE_SIZE);
                    // --- [END AI-GENERATED] ---

                    ImGui::SetCursorScreenPos(boardStartPos);

                    ImGui::BeginChild("PiecesLayer", ImVec2(TILE_SIZE * 8, TILE_SIZE * 8), false, ImGuiWindowFlags_NoBackground);
                    {
                        if (game_state.is_finished)
                        {
                            to_be_promoted = false;
                        }

                        promotion_screen(to_be_promoted, pieces);

                        draw_pieces(game_state, logger, to_be_promoted, pieces, TAB_POS, current_piece, current_piece_id);
                    }
                    ImGui::EndChild();

                    if (game_state.in_menu || game_state.is_finished)
                    {
                        game_menu(boardStartPos, game_state, logger, TILE_SIZE, to_be_promoted, pieces, TAB_POS, current_piece, current_piece_id);
                    }

                    // --- Logs ---
                    logger.Draw(120.f);

                    // --- [BEGIN AI-GENERATED] ENDLESS FALL TICK ---
                    tick_wonderland_timer(game_state, logger, current_piece, current_piece_id, pieces);
                    // --- [END AI-GENERATED] ---

                    // --- WINDOW: ECHOES OF WONDERLAND ---
                    draw_wonderland_echo_window(game_state);

                    // --- [BEGIN AI-GENERATED] WINDOW: STATISTICS ---
                    draw_wonderland_stats_window(game_state);
                    // --- [END AI-GENERATED] ---

                    ImGui::End();
                },
        }
    );
}
