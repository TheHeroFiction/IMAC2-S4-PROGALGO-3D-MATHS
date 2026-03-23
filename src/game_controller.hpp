#pragma once
#include "chess_pieces.hpp"
#include "game_state.hpp"
#include "logger.hpp"

void show_mode_selection(ImVec2 boardStartPos, GameState& game_state, GameLogger& logger, float TILE_SIZE, std::vector<Piece>& pieces, const std::map<std::string, ImVec2>& TAB_POS, std::pair<std::string, PIECE_STATUS>& current_piece, int current_piece_id);
void promotion_screen(bool& to_be_promoted, std::vector<Piece>& pieces);