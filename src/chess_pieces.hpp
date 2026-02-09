#pragma once
#include <imgui.h>
#include <map>
#include <string>
#include <vector>

class Piece {
private:
    std::string m_name;
    bool        m_is_white;
    bool        m_is_playable;
    std::string m_current_case;
    ImVec2      m_position;
    float       m_tile_size;

public:
    // constructor
    Piece();
    Piece(Piece const& piece) = default;
    Piece(std::string& name, bool is_white);

    // destructor
    ~Piece() = default;

    // setters
    void set_name(std::string name);
    void set_color(bool is_white);
    void set_current_case(std::string next_case);
    void set_position(ImVec2 new_pos);
    void set_tile_size(float tile_size);

    // getters
    std::string get_name();
    std::string get_current_case();
    ImVec2      get_position();
    bool        is_white() const;

    // others
    bool show_piece();
};

std::vector<Piece> pieces_gen_v2(float tile_size);

void assign_pos_pieces(std::vector<Piece>& pieces, std::map<std::string, ImVec2> tab_pos);