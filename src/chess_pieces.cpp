#include "chess_pieces.hpp"
#include <imgui.h>
#include <string>
#include <vector>

// constructor
Piece::Piece()
    : m_name(""), m_is_white(false), m_is_playable(true) {}

Piece::Piece(Piece* piece)
    : m_name(piece->m_name), m_is_white(piece->m_is_white), m_is_playable(piece->m_is_playable)
{}

Piece::Piece(std::string name, bool is_white)
    : m_name(name), m_is_white(is_white), m_is_playable(true) {}

// destructor
Piece::~Piece() {}

// setters
void Piece::set_name(std::string name)
{
    m_name = name;
}

void Piece::set_color(bool is_white)
{
    m_is_white = is_white;
}

void Piece::set_current_case(std::string next_case)
{
    m_current_case = next_case;
}

// getters
std::string Piece::get_name()
{
    return m_name;
}

std::string Piece::get_current_case()
{
    return m_current_case;
}

// others
void Piece::show_piece()
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{static_cast<float>(m_is_white), static_cast<float>(m_is_white), static_cast<float>(m_is_white), 1.f});
    ImGui::Button(m_name.c_str(), ImVec2{50.f, 50.f});
    ImGui::PopStyleColor();
}

std::vector<Piece> pieces_gen_v2()
{
    std::vector<Piece> pieces;
    for (int i{0}; i < 2; i++)
    {
        for (int j{0}; j < 16; j++)
        {
            Piece piece{};

            std::string name{};
            if (i == 0)
            {
                name.push_back('D'); // Dark aka Black
            }
            else
            {
                name.push_back('W'); // White
            }

            if (j < 8)
            {
                name.push_back('P'); // Pawn
                name += std::to_string(j);
            }
            else if (j < 10)
            {
                name.push_back('R'); // Rook
                name += std::to_string(j - 8);
            }
            else if (j < 12)
            {
                name.push_back('H'); // Horse aka Knight
                name += std::to_string(j - 10);
            }
            else if (j < 14)
            {
                name.push_back('B'); // Bishop
                name += std::to_string(j - 12);
            }
            else if (j == 14)
            {
                name.push_back('Q'); // Queen
            }
            else
            {
                name.push_back('K'); // King
            }
            piece.set_name(name);
            piece.set_color(static_cast<bool>(i));
            pieces.push_back(piece);
        }
    }
    return pieces;
}