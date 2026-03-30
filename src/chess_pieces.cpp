#include "chess_pieces.hpp"
#include <string>
#include <utility>
#include <vector>

const std::vector<Behaviour>   titles{Behaviour::Bishop, Behaviour::Knight, Behaviour::Rook, Behaviour::Queen};
const std::vector<std::string> titles_names{"Bishop", "Knight", "Rook", "Queen"};

// constructor
Piece::Piece()
    : m_name(""), m_is_white(false), m_is_playable(true) {}

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

void Piece::set_current_tile(std::string next_tile)
{
    m_current_tile = next_tile;
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

void Piece::set_texture_id(unsigned int id)
{
    m_texture_id = id;
}

// getters
std::string Piece::get_name() const
{
    return m_name;
}

std::string Piece::get_current_tile() const
{
    return m_current_tile;
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

unsigned int Piece::get_texture_id() const
{
    return m_texture_id;
}

Behaviour Piece::get_behaviour() const
{
    return m_behaviour;
}

// others

bool Piece::show_piece(std::pair<std::string, PIECE_STATUS>& current_piece, bool& is_game_finished, bool& is_white_turn, std::vector<Piece>& all_pieces, const std::map<std::string, ImVec2>& tab_pos, bool& to_be_promoted)
{
    bool clicked = false;

    ImVec2 piece_screen_pos = ImGui::GetCursorScreenPos();

    ImVec2 board_origin = ImVec2(piece_screen_pos.x - m_position.x, piece_screen_pos.y - m_position.y);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));

    if (ImGui::ImageButton(
            m_name.c_str(),
            (void*)(intptr_t)m_texture_id, // Cast (void*)(intptr_t) obligatoire pour ImGui, c'est comme ça qu'il lit les IDs de texture OpenGL.
            ImVec2(m_tile_size, m_tile_size),
            ImVec2(0, 0), ImVec2(1, 1), // Coordonnées UV (on prend toute l'image)
            ImVec4(0, 0, 0, 0),         // Background (transparent)
            ImVec4(1, 1, 1, 1)          // Teinte (blanche par défaut pour ne pas altérer l'image)
        ))
    {
        clicked = true;
    }

    ImGui::PopStyleColor(1);

    ImGui::PopStyleVar();

    // --- DESSIN DES POSSIBILITÉS (CARRÉS ROUGES) ---
    if (current_piece.first == m_name && current_piece.second == PIECE_STATUS::SELECTED)
    {
        std::vector<std::string> valid_tiles               = get_possible_moves(all_pieces);
        std::string              label_for_multiple_pieces = "";

        for (const std::string& tile_name : valid_tiles)
        {
            if (tab_pos.find(tile_name) != tab_pos.end())
            {
                ImVec2 target_pos = tab_pos.at(tile_name);

                ImGui::SetCursorPos(target_pos);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.f, 0.f, 0.f, 1.f});
                if (ImGui::Button(label_for_multiple_pieces.c_str(), ImVec2(m_tile_size, m_tile_size)))
                {
                    current_piece = std::pair("", PIECE_STATUS::UNSELECTED);
                    for (int i{0}; i < all_pieces.size(); i++)
                    {
                        if (all_pieces[i].is_playable() && all_pieces[i].get_current_tile() == tile_name)
                        {
                            all_pieces[i].m_is_playable = false;

                            if (all_pieces[i].get_name() == "WK" || all_pieces[i].get_name() == "DK")
                            {
                                is_game_finished = true;
                            }

                            break;
                        }
                    }
                    if (((m_is_white && tile_name[1] == '8') || (!(m_is_white) && tile_name[1] == '1')) && m_behaviour == Behaviour::Pawn)
                    {
                        to_be_promoted = true;
                    }
                    m_position     = target_pos;
                    m_current_tile = tile_name;
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

    char file = m_current_tile[0];
    int  rank = m_current_tile[1] - '0';

    auto add_sliding_moves = [&](int d_file, int d_rank) {
        for (int i = 1; i < 8; i++)
        {
            char f = file + (d_file * i);
            int  r = rank + (d_rank * i);
            if (f < 'a' || f > 'h' || r < 1 || r > 8)
                break; // Sortie du plateau

            const Piece* p      = get_piece_at(f, r, board_pieces);
            std::string  target = std::string(1, f) + std::to_string(r);

            if (p == nullptr || (p->is_white() == m_is_white && !(p->is_playable())))
            {
                possible_moves.push_back(target); // tile vide, on peut y aller
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

        // Avancer d'une tile
        if (get_piece_at(file, rank + dir, board_pieces) == nullptr)
        {
            possible_moves.push_back(std::string(1, file) + std::to_string(rank + dir));
            // Avancer de deux tiles (seulement si la première est vide et qu'on est sur le départ)
            if (rank == start_rank && get_piece_at(file, rank + (dir * 2), board_pieces) == nullptr)
            {
                possible_moves.push_back(std::string(1, file) + std::to_string(rank + (dir * 2)));
            }
        }
        // Manger en diagonale
        const Piece* diag_left = get_piece_at(file - 1, rank + dir, board_pieces);
        if (diag_left != nullptr && diag_left->is_white() != m_is_white)
            possible_moves.push_back(std::string(1, file - 1) + std::to_string(rank + dir));

        const Piece* diag_right = get_piece_at(file + 1, rank + dir, board_pieces);
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
                const Piece* p = get_piece_at(f, r, board_pieces);
                if (p == nullptr || p->is_white() != m_is_white || (p->is_white() == m_is_white && !(p->is_playable())))
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
                const Piece* p = get_piece_at(f, r, board_pieces);
                if (p == nullptr || p->is_white() != m_is_white || (p->is_white() == m_is_white && !(p->is_playable())))
                    possible_moves.push_back(std::string(1, f) + std::to_string(r));
            }
        }
        break;
    }
    }
    return possible_moves;
}

const Piece* Piece::get_piece_at(char f, int r, const std::vector<Piece>& board_pieces)
{
    if (f < 'a' || f > 'h' || r < 1 || r > 8)
        return nullptr; // Hors du plateau
    std::string target = std::string(1, f) + std::to_string(r);
    for (const auto& p : board_pieces)
    {
        if (p.get_current_tile() == target && p.is_playable())
            return &p;
    }
    return nullptr;
};
