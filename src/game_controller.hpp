#pragma once
#include "chess_pieces.hpp"
#include "game_state.hpp"
#include "logger.hpp"
#include "utils.hpp"

void               show_mode_selection(ImVec2 boardStartPos, GameState& game_state, GameLogger& logger, float TILE_SIZE, std::vector<Piece>& pieces, const std::map<std::string, ImVec2>& TAB_POS, std::pair<std::string, PIECE_STATUS>& current_piece, int current_piece_id);
void               promotion_screen(bool& to_be_promoted, std::vector<Piece>& pieces);
void               draw_pieces(GameState& game_state, GameLogger& logger, bool& to_be_promoted, std::vector<Piece>& pieces, const std::map<std::string, ImVec2>& TAB_POS, std::pair<std::string, PIECE_STATUS>& current_piece, int current_piece_id);
std::vector<Piece> pieces_gen(float tile_size);
void               assign_pos_pieces(std::vector<Piece>& pieces, std::map<std::string, ImVec2> tab_pos);
void               config_piece(int color, int j, Piece& piece, std::string& name, float tile_size);
unsigned int       give_texture_id(Behaviour Piece_Behaviour, int i);