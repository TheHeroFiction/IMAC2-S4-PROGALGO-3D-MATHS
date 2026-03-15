#include "utils.hpp"
#include <imgui.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

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

GLuint load_texture_from_file(const char* filename)
{
    int image_width = 0;
    int image_height = 0;
    int channels = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, &channels, 4);
    
    if (image_data == nullptr)
    {
        std::cout << "Erreur : Impossible de charger l'image " << filename << "\n";
        return 0;
    }

    GLuint image_texture = 0;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Paramètres pour redimensionner l'image proprement
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    stbi_image_free(image_data);

    return image_texture;
}