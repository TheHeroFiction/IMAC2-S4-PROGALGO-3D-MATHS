#include "wonderland_runtime.hpp"
#include "wonderland_effects.hpp"

// --- [BEGIN AI-GENERATED] WONDERLAND RUNTIME HELPERS ---
// --- CHESHIRE CAT (POISSON LAW) ---
void draw_wonderland_traps(const GameState& game_state, const std::map<std::string, ImVec2>& tab_pos, const ImVec2& board_start_pos, float tile_size)
{
    if (game_state.is_wonderland_mode && !game_state.trapped_tiles.empty())
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        for (const std::string& trap_tile : game_state.trapped_tiles)
        {
            if (tab_pos.contains(trap_tile))
            {
                ImVec2 pos = tab_pos.at(trap_tile);

                ImVec2 p_min = ImVec2(board_start_pos.x + pos.x, board_start_pos.y + pos.y);
                ImVec2 p_max = ImVec2(p_min.x + tile_size, p_min.y + tile_size);
                draw_list->AddRectFilled(p_min, p_max, IM_COL32(150, 50, 200, 255));
            }
        }
    }
}

// --- ENDLESS FALL (EXPONENTIAL LAW) ---
void tick_wonderland_timer(GameState& game_state, GameLogger& logger, std::pair<std::string, PIECE_STATUS>& current_piece, int& current_piece_id, std::vector<Piece>& pieces)
{
    if (game_state.is_wonderland_mode && !game_state.is_finished && !game_state.in_menu)
    {
        game_state.turn_timer += ImGui::GetIO().DeltaTime;

        if (game_state.turn_timer >= game_state.current_time_limit)
        {
            logger.AddLog("[Endless Fall] Time's up! Turn skipped.");
            current_piece    = {"", PIECE_STATUS::UNSELECTED};
            current_piece_id = 32;

            game_state.end_turn();

            apply_weather_effects(game_state, pieces);

            game_state.trigger_echo(WonderlandLore::Event::ENDLESS_FALL);
        }
    }
}

// --- WONDERLAND ECHO UI ---
void draw_wonderland_echo_window(const GameState& game_state)
{
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
}
// --- [END AI-GENERATED] ---