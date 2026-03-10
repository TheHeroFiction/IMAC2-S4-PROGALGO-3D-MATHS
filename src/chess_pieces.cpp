#include "chess_pieces.hpp"
#include <imgui.h>
#include <iostream>
#include <string>
#include <utility>
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

void Piece::set_behaviour(Behaviour behaviour)
{
    m_behaviour = behaviour;
}

void Piece::set_playability(bool state)
{
    m_is_playable = state;
}
// getters
std::string Piece::get_name() const
{
    return m_name;
}

std::string Piece::get_current_case() const
{
    return m_current_case;
}

ImVec2 Piece::get_position() const
{
    return m_position;
}

bool Piece::is_white() const
{
    return m_is_white;
}

bool Piece::is_playable() const
{
    return m_is_playable;
}
// others

// bool Piece::show_piece(std::pair<std::string, PIECE_STATUS>& current_piece, bool& is_white_turn)
// {
//     bool clicked = false;
//     ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{static_cast<float>(m_is_white), static_cast<float>(m_is_white), static_cast<float>(m_is_white), 1.f});
//     if (ImGui::Button(m_name.c_str(), ImVec2{m_tile_size, m_tile_size}))
//     {
//         std::cout << m_name << '\n';
//         clicked = true;
//     };
//     ImGui::PopStyleColor();

//     // How the piece can move
//     if (m_behaviour == Behaviour::Pawn && current_piece == std::pair(m_name, PIECE_STATUS::SELECTED))
//     {
//         ImGui::SetCursorPos(ImVec2(m_position.x, 100.f));
//         ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.f, 0.f, 0.f, 1.f});
//         if (ImGui::Button("possiblity", ImVec2(m_tile_size, m_tile_size)))
//         {
//             current_piece = std::pair("", PIECE_STATUS::UNSELECTED);
//             is_white_turn = !is_white_turn;
//         }
//         ImGui::PopStyleColor();
//     }

//     if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
//     {
//         current_piece = std::pair("", PIECE_STATUS::UNSELECTED);
//     }

//     return clicked;
// }

bool Piece::show_piece(std::pair<std::string, PIECE_STATUS>& current_piece, bool& is_white_turn, std::vector<Piece>& all_pieces, const std::map<std::string, ImVec2>& tab_pos)
{
    bool clicked = false;

    ImVec2 piece_screen_pos = ImGui::GetCursorScreenPos();

    ImVec2 board_origin = ImVec2(piece_screen_pos.x - m_position.x, piece_screen_pos.y - m_position.y);

    // --- DESSIN DE LA PIÈCE ---
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{static_cast<float>(m_is_white), static_cast<float>(m_is_white), static_cast<float>(m_is_white), 0.8f});
    if (ImGui::Button(m_name.c_str(), ImVec2{m_tile_size, m_tile_size}))
    {
        clicked = true;
    };
    ImGui::PopStyleColor();

    // --- DESSIN DES POSSIBILITÉS (CARRÉS ROUGES) ---
    if (current_piece.first == m_name && current_piece.second == PIECE_STATUS::SELECTED)
    {
        std::vector<std::string> valid_cases = get_possible_moves(all_pieces);
        // ImDrawList*              draw_list   = ImGui::GetWindowDrawList();
        std::string label_for_multiple_pieces = "";

        for (const std::string& case_name : valid_cases)
        {
            if (tab_pos.find(case_name) != tab_pos.end())
            {
                ImVec2 target_pos = tab_pos.at(case_name);

                ImGui::SetCursorPos(target_pos);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.f, 0.f, 0.f, 1.f});
                if (ImGui::Button(label_for_multiple_pieces.c_str(), ImVec2(m_tile_size, m_tile_size)))
                {
                    current_piece = std::pair("", PIECE_STATUS::UNSELECTED);
                    for (int i{0}; i < all_pieces.size(); i++)
                    {
                        if (all_pieces[i].is_playable() && all_pieces[i].get_current_case() == case_name)
                        {
                            all_pieces[i].m_is_playable = false;
                            break;
                        }
                    }
                    m_position     = target_pos;
                    m_current_case = case_name;
                    is_white_turn  = !is_white_turn;
                }
                ImGui::PopStyleColor();
            }
            label_for_multiple_pieces.append(" ");
        }
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        current_piece = std::pair("", PIECE_STATUS::UNSELECTED);
    }

    return clicked;
}

std::vector<std::string> Piece::get_possible_moves(const std::vector<Piece>& board_pieces)
{
    std::vector<std::string> possible_moves;

    char file = m_current_case[0];
    int  rank = m_current_case[1] - '0';

    auto get_piece_at = [&](char f, int r) -> const Piece* {
        if (f < 'a' || f > 'h' || r < 1 || r > 8)
            return nullptr; // Hors du plateau
        std::string target = std::string(1, f) + std::to_string(r);
        for (const auto& p : board_pieces)
        {
            if (p.get_current_case() == target)
                return &p;
        }
        return nullptr;
    };

    auto add_sliding_moves = [&](int d_file, int d_rank) {
        for (int i = 1; i < 8; i++)
        {
            char f = file + (d_file * i);
            int  r = rank + (d_rank * i);
            if (f < 'a' || f > 'h' || r < 1 || r > 8)
                break; // Sortie du plateau

            const Piece* p      = get_piece_at(f, r);
            std::string  target = std::string(1, f) + std::to_string(r);

            if (p == nullptr)
            {
                possible_moves.push_back(target); // Case vide, on peut y aller
            }
            else
            {
                if (p->is_white() != m_is_white)
                    possible_moves.push_back(target); // Ennemi : on peut manger, mais on s'arrête
                break;                                // Bloqué par une pièce (amie ou ennemie)
            }
        }
    };

    switch (m_behaviour)
    {
    case Behaviour::Pawn:
    {
        int dir        = m_is_white ? 1 : -1; // Les blancs montent (+1), les noirs descendent (-1)
        int start_rank = m_is_white ? 2 : 7;

        // Avancer d'une case
        if (get_piece_at(file, rank + dir) == nullptr)
        {
            possible_moves.push_back(std::string(1, file) + std::to_string(rank + dir));
            // Avancer de deux cases (seulement si la première est vide et qu'on est sur le départ)
            if (rank == start_rank && get_piece_at(file, rank + (dir * 2)) == nullptr)
            {
                possible_moves.push_back(std::string(1, file) + std::to_string(rank + (dir * 2)));
            }
        }
        // Manger en diagonale
        const Piece* diag_left = get_piece_at(file - 1, rank + dir);
        if (diag_left != nullptr && diag_left->is_white() != m_is_white)
            possible_moves.push_back(std::string(1, file - 1) + std::to_string(rank + dir));

        const Piece* diag_right = get_piece_at(file + 1, rank + dir);
        if (diag_right != nullptr && diag_right->is_white() != m_is_white)
            possible_moves.push_back(std::string(1, file + 1) + std::to_string(rank + dir));
        break;
    }
    case Behaviour::Knight:
    {
        // Les 8 sauts en "L" du cavalier
        int moves[8][2] = {{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};
        for (auto& m : moves)
        {
            char f = file + m[0];
            int  r = rank + m[1];
            if (f >= 'a' && f <= 'h' && r >= 1 && r <= 8)
            {
                const Piece* p = get_piece_at(f, r);
                if (p == nullptr || p->is_white() != m_is_white)
                    possible_moves.push_back(std::string(1, f) + std::to_string(r));
            }
        }
        break;
    }
    case Behaviour::Rook:
        add_sliding_moves(1, 0);
        add_sliding_moves(-1, 0);
        add_sliding_moves(0, 1);
        add_sliding_moves(0, -1);
        break;
    case Behaviour::Bishop:
        add_sliding_moves(1, 1);
        add_sliding_moves(1, -1);
        add_sliding_moves(-1, 1);
        add_sliding_moves(-1, -1);
        break;
    case Behaviour::Queen:
        add_sliding_moves(1, 0);
        add_sliding_moves(-1, 0);
        add_sliding_moves(0, 1);
        add_sliding_moves(0, -1);
        add_sliding_moves(1, 1);
        add_sliding_moves(1, -1);
        add_sliding_moves(-1, 1);
        add_sliding_moves(-1, -1);
        break;
    case Behaviour::King:
    {
        int moves[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (auto& m : moves)
        {
            char f = file + m[0];
            int  r = rank + m[1];
            if (f >= 'a' && f <= 'h' && r >= 1 && r <= 8)
            {
                const Piece* p = get_piece_at(f, r);
                if (p == nullptr || p->is_white() != m_is_white)
                    possible_moves.push_back(std::string(1, f) + std::to_string(r));
            }
        }
        break;
    }
    }
    return possible_moves;
}

std::vector<Piece> pieces_gen(float tile_size)
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
                piece.set_behaviour(Behaviour::Pawn);
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
                piece.set_behaviour(Behaviour::Rook);
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
                piece.set_behaviour(Behaviour::Knight);
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
                piece.set_behaviour(Behaviour::Bishop);
            }
            else if (j == 14)
            {
                name.push_back('Q'); // Queen
                piece.set_current_case(std::string(1, static_cast<char>('d')) + piece.get_current_case());
                piece.set_behaviour(Behaviour::Queen);
            }
            else
            {
                name.push_back('K'); // King
                piece.set_current_case(std::string(1, static_cast<char>('e')) + piece.get_current_case());
                piece.set_behaviour(Behaviour::King);
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