#pragma once
#include <string>

struct GameState {
    bool is_white_turn = true; // Les blancs commencent toujours

    // Fonction pour changer le tour
    void end_turn()
    {
        is_white_turn = !is_white_turn;
    }

    // Fonction pour l'affichage
    std::string get_current_player_name() const
    {
        if (is_white_turn)
        {
            return "Blancs";
        }
        else
        {
            return "Noirs";
        }
    }
};