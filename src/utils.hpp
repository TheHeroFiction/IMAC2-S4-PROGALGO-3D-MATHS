#pragma once
#include <imgui.h>
#include <map>
#include <string>
#include <glad/glad.h>

std::map<std::string, ImVec2> generate_tab_position(float const tile_size);

GLuint load_texture_from_file(const char* filename);