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

                    // --- Header ---
                    game_state.game_header();

                    // --- Le Plateau et les Pièces ---
                    ImVec2 boardStartPos = ImGui::GetCursorScreenPos();

                    draw_board(TILE_SIZE);

                    ImGui::SetCursorScreenPos(boardStartPos);

                    ImGui::BeginChild("PiecesLayer", ImVec2(TILE_SIZE * 8, TILE_SIZE * 8), false, ImGuiWindowFlags_NoBackground);
                    {
                        promotion_screen(to_be_promoted, pieces);

                        draw_pieces(game_state, logger, to_be_promoted, pieces, TAB_POS, current_piece, current_piece_id);
                    }
                    ImGui::EndChild();

                    if (game_state.in_menu || game_state.is_finished)
                    {
                        show_mode_selection(boardStartPos, game_state, logger, TILE_SIZE, pieces, TAB_POS, current_piece, current_piece_id);
                    }

                    // --- Logs ---
                    logger.Draw(120.f);

                    ImGui::End();
                },
        }
    );
}
