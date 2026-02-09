#include "chess_pieces.hpp"
#include <imgui.h>
#include <iostream>
#include <string>
#include <vector>

// constructor
Piece::Piece()
    : m_name(""), m_is_white(false), m_is_playable(true) {}

// Piece::Piece(Piece const& piece)

// {}

Piece::Piece(std::string& name, bool is_white)
    : m_name(name), m_is_white(is_white), m_is_playable(true) {}

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

void Piece::set_position(ImVec2 new_pos)
{
    m_position = new_pos;
}

void Piece::set_tile_size(float tile_size)
{
    m_tile_size = tile_size;
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

ImVec2 Piece::get_position()
{
    return m_position;
}

bool Piece::is_white() const
{
    return m_is_white;
}

// others
bool Piece::show_piece()
{
    bool clicked = false;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{static_cast<float>(m_is_white), static_cast<float>(m_is_white), static_cast<float>(m_is_white), 1.f});
    if (ImGui::Button(m_name.c_str(), ImVec2{50.f, 50.f}))
    {
        std::cout << m_name << '\n';
        clicked = true;
    };
    ImGui::PopStyleColor();

    return clicked;
}

std::vector<Piece> pieces_gen_v2(float tile_size)
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
                piece.set_current_case("7");
                if (j > 7)
                {
                    piece.set_current_case("8");
                }
            }
            else
            {
                name.push_back('W'); // White
                piece.set_current_case("2");
                if (j > 7)
                {
                    piece.set_current_case("1");
                }
            }

            std::cout << "test number: " << piece.get_current_case() << std::endl;

            if (j < 8)
            {
                name.push_back('P'); // Pawn
                name += std::to_string(j);

                piece.set_current_case(std::string(1, static_cast<char>('a' + j)) + piece.get_current_case());
            }
            else if (j < 10)
            {
                name.push_back('R'); // Rook
                name += std::to_string(j - 8);

                if (j == 8)
                {
                    piece.set_current_case(std::string(1, static_cast<char>('a')) + piece.get_current_case());
                }
                else
                {
                    piece.set_current_case(std::string(1, static_cast<char>('h')) + piece.get_current_case());
                }
            }
            else if (j < 12)
            {
                name.push_back('H'); // Horse aka Knight
                name += std::to_string(j - 10);

                if (j == 10)
                {
                    piece.set_current_case(std::string(1, static_cast<char>('b')) + piece.get_current_case());
                }
                else
                {
                    piece.set_current_case(std::string(1, static_cast<char>('g')) + piece.get_current_case());
                }
            }
            else if (j < 14)
            {
                name.push_back('B'); // Bishop
                name += std::to_string(j - 12);

                if (j == 12)
                {
                    piece.set_current_case(std::string(1, static_cast<char>('c')) + piece.get_current_case());
                }
                else
                {
                    piece.set_current_case(std::string(1, static_cast<char>('f')) + piece.get_current_case());
                }
            }
            else if (j == 14)
            {
                name.push_back('Q'); // Queen
                piece.set_current_case(std::string(1, static_cast<char>('d')) + piece.get_current_case());
            }
            else
            {
                name.push_back('K'); // King
                piece.set_current_case(std::string(1, static_cast<char>('e')) + piece.get_current_case());
            }

            std::cout << "nom piece " << name << " ;test final:" << piece.get_current_case() << std::endl;
            piece.set_name(name);
            piece.set_color(static_cast<bool>(i));
            piece.set_tile_size(tile_size);
            pieces.push_back(piece);
        }
    }
    std::cout << "test pre retour: " << pieces[0].get_name() << "; case:" << pieces[0].get_current_case() << std::endl;
    return pieces;
}

void assign_pos_pieces(std::vector<Piece>& pieces, std::map<std::string, ImVec2> tab_pos)
{
    for (int i{0}; i < pieces.size(); i++)
    {
        pieces[i].set_position(tab_pos[pieces[i].get_current_case()]);
    }
}