#include "utils.hpp"
#include <imgui.h>

std::map<std::string, ImVec2> generate_tab_position(float const tile_size)
{
    std::map<std::string, ImVec2> tab_pos;
    for (int x{0}; x < 8; x++)
    {
        for (int y{0}; y < 8; y++)
        {
            // Position
            char file = static_cast<char>('a' + x);

            int rank = 8 - y;

            std::string tile_name = std::string(1, file) + std::to_string(rank);

            tab_pos[tile_name] = ImVec2(x * tile_size, y * tile_size);
        }
    }
    return tab_pos;
}