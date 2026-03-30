#pragma once
#include <glad/glad.h>
#include <imgui.h>
#include <map>
#include <string>

std::map<std::string, ImVec2> generate_tab_position(float tile_size);

GLuint load_texture_from_file(const char* filename);