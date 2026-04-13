#pragma once
#include <imgui.h>
#include <array>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include "game_state.hpp"

enum class Behaviour {
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King
};

enum class PIECE_STATUS {
    SELECTED,
    UNSELECTED,
};

class Piece {
private:
    std::string  m_name;
    bool         m_is_white;
    bool         m_is_playable;
    std::string  m_current_tile;
    ImVec2       m_position;
    float        m_tile_size;
    Behaviour    m_behaviour;
    unsigned int m_texture_id;
    float        m_magic_scale = 1.0f;

public:
    // --- CONSTRUCTORS ---
    Piece();
    Piece(Piece const& piece) = default;
    Piece(std::string& name, bool is_white);

    // --- DESTRUCTOR ---
    ~Piece() = default;

    // --- SETTERS ---
    void set_name(std::string name);
    void set_color(bool is_white);
    void set_current_tile(std::string next_tile);
    void set_position(ImVec2 new_pos);
    void set_tile_size(float tile_size);
    void set_behaviour(Behaviour behaviour);
    void set_playability(bool state);
    void set_texture_id(unsigned int id);
    void set_magic_scale(float scale);

    // --- GETTERS ---
    std::string  get_name() const;
    std::string  get_current_tile() const;
    ImVec2       get_position() const;
    bool         is_white() const;
    bool         is_playable() const;
    unsigned int get_texture_id() const;
    Behaviour    get_behaviour() const;
    float get_magic_scale() const;

    // --- OPERATORS ---

    // --- OTHERS ---
    bool show_piece(std::pair<std::string, PIECE_STATUS>& current_piece, GameState& game_state, std::vector<Piece>& all_pieces, const std::map<std::string, ImVec2>& tab_pos, bool& to_be_promoted);

    std::vector<std::string> get_possible_moves(const std::vector<Piece>& board_pieces);
    const Piece*             get_piece_at(char f, int r, const std::vector<Piece>& board_pieces);
};
