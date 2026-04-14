#pragma once

#include <imgui.h>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include "chess_pieces.hpp"
#include "logger.hpp"

// --- [BEGIN AI-GENERATED] WONDERLAND RUNTIME API ---
void draw_wonderland_traps(const GameState& game_state, const std::map<std::string, ImVec2>& tab_pos, const ImVec2& board_start_pos, float tile_size);
void tick_wonderland_timer(GameState& game_state, GameLogger& logger, std::pair<std::string, PIECE_STATUS>& current_piece, int& current_piece_id, std::vector<Piece>& pieces);
void draw_wonderland_echo_window(const GameState& game_state);
// --- [END AI-GENERATED] ---