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

                    // --- WINDOW: ECHOES OF WONDERLAND ---
                    if (game_state.is_wonderland_mode && game_state.current_event != WonderlandLore::Event::NONE)
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
