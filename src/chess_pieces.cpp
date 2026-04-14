#include "chess_pieces.hpp"
#include "game_controller.hpp"

// --- CONSTRUCTOR ---
Piece::Piece()
    : m_name(""), m_is_white(false), m_is_playable(true) {}

Piece::Piece(std::string& name, bool is_white)
    : m_name(name), m_is_white(is_white), m_is_playable(true) {}

// --- SETTERS ---
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

void Piece::set_magic_scale(float scale)
{
    m_magic_scale = scale;
}

void Piece::set_painted_red(bool state)
{
    m_is_painted_red = state;
}

// --- GETTERS ---
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

float Piece::get_magic_scale() const
{
    return m_magic_scale;
}

bool Piece::is_painted_red() const
{
    return m_is_painted_red;
}

// --- OTHERS ---
bool Piece::show_piece(std::pair<std::string, PIECE_STATUS>& current_piece, GameState& game_state, std::vector<Piece>& all_pieces, const std::map<std::string, ImVec2>& tab_pos, bool& to_be_promoted)
{
    bool   clicked          = false;
    ImVec2 piece_screen_pos = ImGui::GetCursorScreenPos();
    ImVec2 board_origin     = ImVec2(piece_screen_pos.x - m_position.x, piece_screen_pos.y - m_position.y);

    // --- WONDERLAND VISUAL LAWS ---
    float  current_tile_size = m_tile_size * m_magic_scale;
    ImVec2 render_offset(0.f, 0.f);

    if (game_state.is_wonderland_mode)
    {
        // Law 6: Normal Distribution (The White Rabbit - Anxiety jitter)
        if (game_state.current_weather == WonderlandLore::Event::WHITE_RABBIT)
        {
            render_offset.x = game_state.alice_engine.get_normal(0.0f, 2.5f);
            render_offset.y = game_state.alice_engine.get_normal(0.0f, 2.5f);
        }
    }
    // ------------------------------

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));

    ImGui::SetCursorPos(ImVec2(m_position.x + render_offset.x, m_position.y + render_offset.y));

    // --- [BEGIN AI-GENERATED] WONDERLAND MATH EFFECTS ---

    // --- DRAW PIECE BUTTON ---
    ImVec4 tint_color = m_is_painted_red ? ImVec4(1.0f, 0.2f, 0.2f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    bool is_correct_turn = (m_is_white == game_state.is_white_turn);
    bool has_moves       = !get_possible_moves(all_pieces, game_state).empty();

    if (ImGui::ImageButton(
            m_name.c_str(),
            (void*)(intptr_t)m_texture_id,
            ImVec2(current_tile_size, current_tile_size),
            ImVec2(0, 0), ImVec2(1, 1),
            ImVec4(0, 0, 0, 0),
            tint_color
        ))
    {
        bool is_correct_turn = (m_is_white == game_state.is_white_turn);

        bool has_moves = !get_possible_moves(all_pieces, game_state).empty();

        if (game_state.is_wonderland_mode && is_correct_turn && has_moves)
        {
            if (m_stubbornness > 0)
            {
                m_stubbornness--;
                game_state.trigger_echo(WonderlandLore::Event::RED_QUEEN);
            }
            else
            {
                clicked        = true;
                m_stubbornness = game_state.alice_engine.get_geometric(0.4f);
            }
        }
        else
        {
            clicked = true;
        }
        // --- [END AI-GENERATED] ---
    }

    ImGui::PopStyleColor(1);

    ImGui::PopStyleVar();

    // --- DRAW POSSIBLE MOVES (RED SQUARES) ---
    if (current_piece.first == m_name && current_piece.second == PIECE_STATUS::SELECTED)
    {
        std::vector<std::string> valid_tiles               = get_possible_moves(all_pieces, game_state);
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

                    // --- [BEGIN AI-GENERATED] CAUCHY LAW (THE LOOKING GLASS) ---
                    std::string final_tile   = tile_name;
                    bool        has_glitched = false;

                    if (game_state.is_wonderland_mode)
                    {
                        int dx = static_cast<int>(std::round(game_state.alice_engine.get_cauchy(0.0f, 0.15f)));
                        int dy = static_cast<int>(std::round(game_state.alice_engine.get_cauchy(0.0f, 0.15f)));

                        if (dx != 0 || dy != 0)
                        {
                            char target_file = tile_name[0];
                            int  target_rank = tile_name[1] - '0';

                            char new_file = std::max('a', std::min('h', static_cast<char>(target_file + dx)));
                            int  new_rank = std::max(1, std::min(8, target_rank + dy));

                            std::string glitched_tile = std::string(1, new_file) + std::to_string(new_rank);

                            bool ally_blocking = false;
                            for (const auto& p : all_pieces)
                            {
                                if (p.is_playable() && p.get_current_tile() == glitched_tile && p.is_white() == m_is_white)
                                {
                                    ally_blocking = true;
                                    break;
                                }
                            }

                            if (!ally_blocking)
                            {
                                final_tile   = glitched_tile;
                                has_glitched = true;
                            }
                        }
                    }

                    for (int i{0}; i < all_pieces.size(); i++)
                    {
                        if (all_pieces[i].is_playable() && all_pieces[i].get_current_tile() == final_tile)
                        {
                            all_pieces[i].set_playability(false);

                            if (all_pieces[i].get_name() == "WK" || all_pieces[i].get_name() == "DK")
                            {
                                game_state.is_finished = true;
                            }
                            break;
                        }
                    }

                    if (((m_is_white && final_tile[1] == '8') || (!(m_is_white) && final_tile[1] == '1')) && m_behaviour == Behaviour::Pawn)
                    {
                        to_be_promoted = true;
                    }

                    m_position     = tab_pos.at(final_tile);
                    m_current_tile = final_tile;

                    game_state.end_turn();

                    apply_weather_effects(game_state, all_pieces);

                    if (has_glitched)
                    {
                        game_state.trigger_echo(WonderlandLore::Event::LOOKING_GLASS);
                    }
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

std::vector<std::string> Piece::get_possible_moves(const std::vector<Piece>& board_pieces, const GameState& game_state)
{
    std::vector<std::string> possible_moves;

    char file = m_current_tile[0];
    int  rank = m_current_tile[1] - '0';

    auto is_trapped = [&](const std::string& target) {
        return std::find(game_state.trapped_tiles.begin(), game_state.trapped_tiles.end(), target) != game_state.trapped_tiles.end();
    };

    auto add_sliding_moves = [&](int d_file, int d_rank) {
        for (int i = 1; i < 8; i++)
        {
            char f = file + (d_file * i);
            int  r = rank + (d_rank * i);
            if (f < 'a' || f > 'h' || r < 1 || r > 8)
                break; // --- OUT OF CHESSBOARD BOUNDS ---

            const Piece* p      = get_piece_at(f, r, board_pieces);
            std::string  target = std::string(1, f) + std::to_string(r);

            if (is_trapped(target)) break;

            if (p == nullptr || (p->is_white() == m_is_white && !(p->is_playable())))
            {
                possible_moves.push_back(target); // --- EMPTY TILE CHECK ---
            }
            else
            {
                if (p->is_white() != m_is_white)
                    possible_moves.push_back(target); // --- ENEMY CHECK ---
                break;                                // --- OTHER PIECES ARE BLOCKING ---
            }
        }
    };

    switch (m_behaviour)
    {
    case Behaviour::Pawn:
    {
        int dir        = m_is_white ? 1 : -1; // --- WHITE GOES UP BY 1 / BLACK GOES DOWN BY 1 ---
        int start_rank = m_is_white ? 2 : 7;

        // --- MOVE BY 1 TILE ---
        std::string move1_target = std::string(1, file) + std::to_string(rank + dir);
        if (!is_trapped(move1_target) && get_piece_at(file, rank + dir, board_pieces) == nullptr)
        {
            possible_moves.push_back(move1_target);
            // --- MOVE BY 2 IF ON START POSITION ---
            std::string move2_target = std::string(1, file) + std::to_string(rank + (dir * 2));
            if (rank == start_rank && !is_trapped(move2_target) && get_piece_at(file, rank + (dir * 2), board_pieces) == nullptr)
            {
                possible_moves.push_back(move2_target);
            }
        }
        // --- DIAGONAL DETETECTION ---
        std::string diag_left_target = std::string(1, file - 1) + std::to_string(rank + dir);
        if (!is_trapped(diag_left_target)) 
        {
            const Piece* diag_left = get_piece_at(file - 1, rank + dir, board_pieces);
            if (diag_left != nullptr && diag_left->is_white() != m_is_white)
                possible_moves.push_back(diag_left_target);
        }

        std::string diag_right_target = std::string(1, file + 1) + std::to_string(rank + dir);
        if (!is_trapped(diag_right_target)) 
        {
            const Piece* diag_right = get_piece_at(file + 1, rank + dir, board_pieces);
            if (diag_right != nullptr && diag_right->is_white() != m_is_white)
                possible_moves.push_back(diag_right_target);
        }
        break;
    }
    case Behaviour::Knight:
    {
        std::array<std::array<int, 2>, 8> moves{
            1, 2,
            2, 1,
            2, -1,
            1, -2,
            -1, -2,
            -2, -1,
            -2, 1,
            -1, 2
        };
        for (auto& m : moves)
        {
            char f = file + m[0];
            int  r = rank + m[1];
            if (f >= 'a' && f <= 'h' && r >= 1 && r <= 8)
            {
                std::string target = std::string(1, f) + std::to_string(r);
                
                // --- [BEGIN AI-GENERATED] CHESHIRE CAT TRAP ---
                if (is_trapped(target)) continue;
                // --- [END AI-GENERATED] ---

                const Piece* p = get_piece_at(f, r, board_pieces);
                if (p == nullptr || p->is_white() != m_is_white || (p->is_white() == m_is_white && !(p->is_playable())))
                    possible_moves.push_back(target);
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
        std::array<std::array<int, 2>, 8> moves{
            1, 0,
            -1, 0,
            0, 1,
            0, -1,
            1, 1,
            1, -1,
            -1, 1,
            -1, -1
        };
        for (auto& m : moves)
        {
            char f = file + m[0];
            int  r = rank + m[1];
            if (f >= 'a' && f <= 'h' && r >= 1 && r <= 8)
            {
                std::string target = std::string(1, f) + std::to_string(r);
                
                // --- [BEGIN AI-GENERATED] CHESHIRE CAT TRAP ---
                if (is_trapped(target)) continue;
                // --- [END AI-GENERATED] ---

                const Piece* p = get_piece_at(f, r, board_pieces);
                if (p == nullptr || p->is_white() != m_is_white || (p->is_white() == m_is_white && !(p->is_playable())))
                    possible_moves.push_back(target);
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
        return nullptr; // --- OUT OF CHESSBOARD BOUNDS ---
    std::string target = std::string(1, f) + std::to_string(r);
    for (const auto& p : board_pieces)
    {
        if (p.get_current_tile() == target && p.is_playable())
            return &p;
    }
    return nullptr;
};
