#pragma once
#include <imgui.h>
#include <string>
#include <vector>

class Piece {
private:
    std::string m_name;
    bool        m_is_white;
    bool        m_is_playable;
    std::string m_current_case;

public:
    // constructor
    Piece();
    Piece(Piece* piece);
    Piece(std::string name, bool is_white);

    // destructor
    ~Piece();

    // setters
    void set_name(std::string name);
    void set_color(bool is_white);
    void set_current_case(std::string next_case);

    // gettters
    std::string get_name();
    std::string get_current_case();

    // others
    void show_piece();
};

std::vector<Piece> pieces_gen_v2();