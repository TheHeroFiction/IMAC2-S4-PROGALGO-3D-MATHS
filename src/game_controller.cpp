#include "game_controller.hpp"
#include <iostream>
#include "renderer3D.hpp"
#include "utils.hpp"
#include "wonderland_effects.hpp"

std::vector<std::pair<std::string, Behaviour>> titles_for_promotions{{"Rook", Behaviour::Rook}, {"Knight", Behaviour::Knight}, {"Bishop", Behaviour::Bishop}, {"Queen", Behaviour::Queen}};

void game_menu(ImVec2 boardStartPos, GameState& game_state, GameLogger& logger, float TILE_SIZE, bool& to_be_promoted, std::vector<Piece>& pieces, const std::map<std::string, ImVec2>& TAB_POS, std::pair<std::string, PIECE_STATUS>& current_piece, int current_piece_id)
{
    ImGui::SetCursorScreenPos(boardStartPos);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4{0.1f, 0.1f, 0.1f, 0.7f});
    ImGui::BeginChild("Menu", ImVec2(TILE_SIZE * 8, TILE_SIZE * 8), false);

    ImGui::SetWindowFontScale(1.3f);

    ImVec2 buttonSize(330.f, 50.f);
    float  spacing = 15.f;

    float total_height = (buttonSize.y * 3) + (spacing * 2);

    float x_offset = (ImGui::GetWindowWidth() - buttonSize.x) * 0.5f;
    float y_offset = (ImGui::GetWindowHeight() - total_height) * 0.5f;

    ImGui::SetCursorPos(ImVec2(x_offset, y_offset));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.2f, 1.0f));

    // --- DEFAULT (2D) GAME ---
    std::string btn_label = (game_state.is_finished && !game_state.is_wonderland_mode) ? "REPLAY 2D GAMEMODE" : "2D GAMEMODE";
    if (ImGui::Button(btn_label.c_str(), buttonSize))
    {
        game_state.in_menu            = false;
        game_state.is_finished        = false;
        game_state.is_white_turn      = true;
        game_state.is_wonderland_mode = false;
        game_state.trigger_echo(WonderlandLore::Event::NONE);
        to_be_promoted = false;

        pieces = pieces_gen(TILE_SIZE);
        assign_pos_pieces(pieces, TAB_POS);

        current_piece    = std::pair("", PIECE_STATUS::UNSELECTED);
        current_piece_id = 32;

        logger.Clear();
        logger.AddLog("GAME START!");
    }

    ImGui::SetCursorPosX(x_offset);
    ImGui::Dummy(ImVec2(0.f, spacing));
    ImGui::SetCursorPosX(x_offset);

    // --- 3D GAMEMODE ---
    if (ImGui::Button("3D GAMEMODE", buttonSize))
    {
        // logger.AddLog("3D GAMEMODE AVAILABLE SOON!");
        main_3D_window(TILE_SIZE * 8);
    }

    ImGui::SetCursorPosX(x_offset);
    ImGui::Dummy(ImVec2(0.f, spacing));
    ImGui::SetCursorPosX(x_offset);

    // --- RANDOM EVENTS GAMEMODE
    std::string random_btn_label = (game_state.is_finished && game_state.is_wonderland_mode) ? "REPLAY RANDOM EVENTS GAMEMODE" : "RANDOM EVENTS GAMEMODE";
    if (ImGui::Button(random_btn_label.c_str(), buttonSize))
    {
        to_be_promoted = false;
        start_wonderland_mode(game_state, pieces, TAB_POS, logger, TILE_SIZE);
        current_piece    = std::pair("", PIECE_STATUS::UNSELECTED);
        current_piece_id = 32;
    }

    ImGui::PopStyleColor(3);
    ImGui::SetWindowFontScale(1.0f);

    ImGui::EndChild();
    ImGui::PopStyleColor();
};

void start_wonderland_mode(GameState& game_state, std::vector<Piece>& pieces, const std::map<std::string, ImVec2>& TAB_POS, GameLogger& logger, float tile_size)
{
    game_state.in_menu            = false;
    game_state.is_finished        = false;
    game_state.is_white_turn      = true;
    game_state.is_wonderland_mode = true;
    game_state.current_weather    = WonderlandLore::Event::NONE;
    game_state.current_echo       = WonderlandLore::Event::NONE;
    game_state.current_quote      = {};
    game_state.trapped_tiles.clear();
    game_state.reset_wonderland_stats();
    game_state.reset_timer();

    pieces = pieces_gen(tile_size);

    // --- MAD HATTER'S TEA PARTY (RANDOM PERMUTATION) ---
    std::vector<int> shuffle_indices = game_state.alice_engine.get_permutation(8);

    for (int color = 0; color < 2; ++color)
    {
        int                offset = color * 16 + 8; // Major pieces are at indices 8-15 and 24-31
        std::vector<Piece> original_back_rank(pieces.begin() + offset, pieces.begin() + offset + 8);

        for (int i = 0; i < 8; ++i)
        {
            pieces[offset + i]   = original_back_rank[shuffle_indices[i]];
            char        new_file = static_cast<char>('a' + i);
            std::string new_tile = std::string(1, new_file) + (color == 0 ? "8" : "1");
            pieces[offset + i].set_current_tile(new_tile);
        }
    }

    assign_pos_pieces(pieces, TAB_POS);
    apply_weather_effects(game_state, pieces);

    logger.Clear();
    logger.AddLog("Down the rabbit hole... Welcome to Wonderland!");
    game_state.trigger_echo(WonderlandLore::Event::MAD_HATTER);
}

void promotion_screen(bool& to_be_promoted, std::vector<Piece>& pieces)
{
    if (to_be_promoted)
    {
        ImGui::OpenPopup("Promote");

        // --- POPUP APPEAR IN CENTER OF SCREEN ---
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Promote", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Choose the promotion you want !");
            ImGui::Separator();

            for (int i{0}; i < titles_for_promotions.size(); i++)
            {
                if (ImGui::Button(titles_for_promotions[i].first.c_str(), ImVec2(120, 0)))
                {
                    to_be_promoted = false;
                    pieces[0].set_behaviour(titles_for_promotions[i].second);
                    pieces[0].set_texture_id(give_texture_id(titles_for_promotions[i].second, pieces[0].is_white()));
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
            }
            ImGui::EndPopup();
        }
    }
}

void draw_pieces(GameState& game_state, GameLogger& logger, bool& to_be_promoted, std::vector<Piece>& pieces, const std::map<std::string, ImVec2>& TAB_POS, std::pair<std::string, PIECE_STATUS>& current_piece, int current_piece_id)
{
    for (int i{0}; i < pieces.size(); i++)
    {
        ImGui::SetCursorPos(pieces[i].get_position());

        ImGui::PushID(i);

        if (pieces[i].is_playable() && pieces[i].show_piece(current_piece, game_state, pieces, TAB_POS, to_be_promoted))
        {
            if (pieces[i].is_white() == game_state.is_white_turn)
            {
                logger.AddLog("Selection : " + pieces[i].get_name());

                current_piece    = {pieces[i].get_name(), PIECE_STATUS::SELECTED};
                current_piece_id = i;
            }
            else
            {
                logger.AddLog("[Error] IT'S NOT YOUR TURN !");
            }
        }
        ImGui::PopID();
    }
    // --- PUT SELECTED PIECE AS FIRST TO BE DRAWN BY IMGUI ---
    if (current_piece_id != 32 && pieces[0].get_name() != pieces[current_piece_id].get_name())
    {
        Piece temp{pieces[current_piece_id]};
        pieces[current_piece_id] = pieces[0];
        pieces[0]                = temp;
        current_piece_id         = 32;
    }
}

std::vector<Piece> pieces_gen(float tile_size)
{
    std::vector<Piece> pieces;
    Piece              piece{};
    std::string        name{};
    for (int color{0}; color < 2; color++)
    {
        for (int j{0}; j < 16; j++)
        {
            name = {};
            config_piece(color, j, piece, name, tile_size);
            pieces.push_back(piece);
        }
    }
    return pieces;
}

void assign_pos_pieces(std::vector<Piece>& pieces, std::map<std::string, ImVec2> tab_pos)
{
    for (int i{0}; i < pieces.size(); i++)
    {
        pieces[i].set_position(tab_pos[pieces[i].get_current_tile()]);
    }
}

void config_piece(int color, int j, Piece& piece, std::string& name, float tile_size)
{
    if (color == 0)
    {
        name.push_back('D'); // --- DARK AKA BLACK ---
        piece.set_current_tile("7");
        if (j > 7)
        {
            piece.set_current_tile("8");
        }
    }
    else
    {
        name.push_back('W'); // --- WHITE ---
        piece.set_current_tile("2");
        if (j > 7)
        {
            piece.set_current_tile("1");
        }
    }

    if (j < 8)
    {
        name.push_back('P'); // --- PAWN ---
        name += std::to_string(j);

        piece.set_current_tile(std::string(1, static_cast<char>('a' + j)) + piece.get_current_tile());
        piece.set_behaviour(Behaviour::Pawn);
    }
    else if (j < 10)
    {
        name.push_back('R'); // --- ROOK ---
        name += std::to_string(j - 8);

        if (j == 8)
        {
            piece.set_current_tile("a" + piece.get_current_tile());
        }
        else
        {
            piece.set_current_tile("h" + piece.get_current_tile());
        }
        piece.set_behaviour(Behaviour::Rook);
    }
    else if (j < 12)
    {
        name.push_back('H'); // --- HORSE AKA KNIGHT ---
        name += std::to_string(j - 10);

        if (j == 10)
        {
            piece.set_current_tile("b" + piece.get_current_tile());
        }
        else
        {
            piece.set_current_tile("g" + piece.get_current_tile());
        }
        piece.set_behaviour(Behaviour::Knight);
    }
    else if (j < 14)
    {
        name.push_back('B'); // --- BISHOP ---
        name += std::to_string(j - 12);

        if (j == 12)
        {
            piece.set_current_tile("c" + piece.get_current_tile());
        }
        else
        {
            piece.set_current_tile("f" + piece.get_current_tile());
        }
        piece.set_behaviour(Behaviour::Bishop);
    }
    else if (j == 14)
    {
        name.push_back('Q'); // --- QUEEN ---
        piece.set_current_tile("d" + piece.get_current_tile());
        piece.set_behaviour(Behaviour::Queen);
    }
    else
    {
        name.push_back('K'); // --- KING ---
        piece.set_current_tile("e" + piece.get_current_tile());
        piece.set_behaviour(Behaviour::King);
    }

    piece.set_texture_id(give_texture_id(piece.get_behaviour(), color));
    piece.set_name(name);
    piece.set_color(static_cast<bool>(color));
    piece.set_tile_size(tile_size);
}

unsigned int give_texture_id(Behaviour Piece_Behaviour, int i)
{
    std::string file_path = "../../chess_pieces_images/";

    switch (Piece_Behaviour)
    {
    case Behaviour::Pawn:
        file_path += "pawn";
        break;
    case Behaviour::Rook:
        file_path += "rook";
        break;
    case Behaviour::Knight:
        file_path += "knight";
        break;
    case Behaviour::Bishop:
        file_path += "bishop";
        break;
    case Behaviour::Queen:
        file_path += "queen";
        break;
    case Behaviour::King:
        file_path += "king";
        break;
    }

    if (i == 0)
    {
        file_path += "_black.png";
    }
    else
    {
        file_path += "_white.png";
    }

    unsigned int tex_id = load_texture_from_file(file_path.c_str());
    return tex_id;
}
