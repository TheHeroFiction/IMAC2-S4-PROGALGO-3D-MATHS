#include <imgui.h>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <utility>
#include <vector>
#include "chess_pieces.hpp"
#include "chessboard.hpp"
#include "game_controller.hpp"
#include "game_ui.hpp"
#include "glimac/Geometry.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#include "glm/matrix.hpp"
#include "quick_imgui/quick_imgui.hpp"
#include "renderer3D.hpp"
#include "utils.hpp"
#include "wonderland_runtime.hpp"
#include "wonderland_stats.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/Program.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/common.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include "glimac/TrackballCamera.hpp"

const float TILE_SIZE = 70.f;

int main(int /*argc*/, char** argv)
{
    std::vector<Piece>                   pieces;
    const std::map<std::string, ImVec2>  TAB_POS{generate_tab_position(TILE_SIZE)};
    std::pair<std::string, PIECE_STATUS> current_piece{"", PIECE_STATUS::UNSELECTED};

    int  current_piece_id{32};
    bool to_be_promoted{false};

    GameState  game_state;
    GameLogger logger;

    int window_width{900};
    int window_height{900};

    TrackballCamera cam{};

    // creation d'un vbo
    GLuint vbo;

    // vertex data for single cube
    float vertices[] = {
        // Back face
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,

        // Front face
        -0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,

        // Left face
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,

        // Right face
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,

        // Bottom face
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,

        // Top face
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
    };

    // creation d'un vao
    GLuint vao;

    // Matrix varaible
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    std::vector<glm::vec3> axes;
    std::vector<int>       rand_pos;

    for (int i{0}; i < 32; i++)
    {
        axes.push_back(glm::sphericalRand(1.f));
        rand_pos.push_back(rand() % 200 + 100);
    }

    glimac::FilePath                   applicationPath(argv[0]);
    std::unique_ptr<BypassBaseProgram> program;
    std::unique_ptr<BypassBaseProgram> program_pieces;

    quick_imgui::loop(
        "Chess",
        {
            .init         = [&]() {
                pieces = pieces_gen(TILE_SIZE);
                assign_pos_pieces(pieces, TAB_POS);
                // add shaders;
                program.reset(new BypassBaseProgram(applicationPath,"shaders/3D.vs.glsl", "shaders/tex3D.fs.glsl"));
                program_pieces.reset(new BypassBaseProgram(applicationPath,"shaders/3D.vs.glsl","shaders/tex3D.fs.glsl"));
                
                // Texture

                std::unique_ptr<glimac::Image> bishop_black = glimac::loadImage(
      applicationPath.dirPath() + "/chess_pieces_images/bishop_black.png");
                
      glGenTextures(1, &program_pieces->uTexture);

  glBindTexture(GL_TEXTURE_2D, program_pieces->uTexture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bishop_black->getWidth(),
               bishop_black->getHeight(), 0, GL_RGBA, GL_FLOAT,
               bishop_black->getPixels());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);
                
                // enable depth test
                glEnable(GL_DEPTH_TEST);
                
                // Create transformation matrices
                // Calculate matrix
                ProjMatrix = glm::perspective(
                    glm::radians(70.f), (window_width * 1.f) / window_height, 0.1f, 100.f
                );
                MVMatrix     = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.f));
                NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
                 glGenVertexArrays(1, &vao);
                glGenBuffers(1, &vbo);

                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
                glEnableVertexAttribArray(0); },
            .loop         = [&]() {
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                        program->m_Program.use();   
                        
                        
                    MVMatrix = cam.getViewMaxtrix();
                    for (int col {0}; col < 8; col++)
                    {
                        for (int row {0}; row < 8; row++)
                        {
                            glm::vec3 color = ((row + col) % 2 == 0) ? glm::vec3(0.1f) : glm::vec3(0.9f);
                            glm::mat4 model = glm::scale(glm::translate(MVMatrix,glm::vec3(col*0.5 -1.5 ,row*0.5 -1.5 ,0)),glm::vec3(0.5,0.5,0.5));
                            // sending matrix to gpu
                            glUniformMatrix4fv(program->uMVMatrix, 1, GL_FALSE, glm::value_ptr(model));
                            glUniformMatrix4fv(
                                program->uNormalMatrix, 1, GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(model))));
                            glUniformMatrix4fv(program->uMVPMatrix, 1, false, glm::value_ptr(ProjMatrix * model));
                            glUniform3fv(program->uTexture, 1,  glm::value_ptr(color));
                            glBindVertexArray(vao);
                            glDrawArrays(GL_TRIANGLES, 0, 36);
                        }
                    }

                    glm::vec3 color =  glm::vec3(0.5f);
                    glm::mat4 model = glm::scale(glm::translate(MVMatrix,glm::vec3( 0.25 ,0.25 ,0)),glm::vec3(4.5,4.5,0.5));
                    // sending matrix to gpu
                    glUniformMatrix4fv(program->uMVMatrix, 1, GL_FALSE, glm::value_ptr(model));
                    glUniformMatrix4fv(
                                program->uNormalMatrix, 1, GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(model))));
                    glUniformMatrix4fv(program->uMVPMatrix, 1, false, glm::value_ptr(ProjMatrix * model));
                    glUniform3fv(program->uTexture, 1,  glm::value_ptr(color));
                    glBindVertexArray(vao);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    
                    program_pieces->m_Program.use();
    
                    for (int col {0}; col < 8; col++)
                    {
                        
                        for (int row {0}; row < 8; row++)
                        {
                            if (row > 1 && row < 6)
                            {
                                continue;
                            }else {
                              model = glm::scale(glm::translate(MVMatrix,glm::vec3( col*0.5 -1.5 ,row*0.5 -1.5,0.5)),glm::vec3(0.2,0.1,0.5));
                            // sending matrix to gpu
                            glUniformMatrix4fv(program_pieces->uMVMatrix, 1, GL_FALSE, glm::value_ptr(model));
                            glUniformMatrix4fv(
                                        program_pieces->uNormalMatrix, 1, GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(model))));
                            glUniformMatrix4fv(program_pieces->uMVPMatrix, 1, false, glm::value_ptr(ProjMatrix * model));
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, program_pieces->uTexture);
                            glBindVertexArray(vao);
                            glDrawArrays(GL_TRIANGLES, 0, 36);
                            }
                        }
                    }
                    // unbind vao
                    glBindVertexArray(0);

                     //ImGui::ShowDemoWindow();
                    
                    ImGui::SetNextWindowPos(ImVec2(20.0f, 64.0f), ImGuiCond_FirstUseEver);
                    ImGui::Begin("Chess Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

                    // --- HEADER ---
                    draw_game_header(game_state);

                    // --- CHESSBOARD & PIECES ---
                    ImVec2 boardStartPos = ImGui::GetCursorScreenPos();

                    draw_board(TILE_SIZE);

                    // --- [BEGIN AI-GENERATED] DRAW CHESHIRE TRAPS ---
                    draw_wonderland_traps(game_state, TAB_POS, boardStartPos, TILE_SIZE);
                    // --- [END AI-GENERATED] ---

                    ImGui::SetCursorScreenPos(boardStartPos);

                    ImGui::BeginChild("PiecesLayer", ImVec2(TILE_SIZE * 8, TILE_SIZE * 8), false, ImGuiWindowFlags_NoBackground);
                    {
                        if (game_state.is_finished)
                        {
                            to_be_promoted = false;
                        }

                        promotion_screen(to_be_promoted, pieces);

                        draw_pieces(game_state, logger, to_be_promoted, pieces, TAB_POS, current_piece, current_piece_id);
                    }
                    ImGui::EndChild();

                    if (game_state.in_menu || game_state.is_finished)
                    {
                        game_menu(boardStartPos, game_state, logger, TILE_SIZE, to_be_promoted, pieces, TAB_POS, current_piece, current_piece_id);
                    }

                    // --- Logs ---
                    logger.Draw(120.f);

                    // --- [BEGIN AI-GENERATED] ENDLESS FALL TICK ---
                    tick_wonderland_timer(game_state, logger, current_piece, current_piece_id, pieces);
                    // --- [END AI-GENERATED] ---

                    // --- WINDOW: ECHOES OF WONDERLAND ---
                    draw_wonderland_echo_window(game_state);

                    // --- [BEGIN AI-GENERATED] WINDOW: STATISTICS ---
                    draw_wonderland_stats_window(game_state);
                    // --- [END AI-GENERATED] ---

                    ImGui::End(); },
            .key_callback = [&](int key, int /*scancode*/, int action, int /*mods*/) { 
                if (key == 81 && action == GLFW_PRESS) 
                {
                    
                    glDeleteBuffers(1, &vbo);
                    glDeleteVertexArrays(1, &vao);
                    ImGui::DestroyContext();
                    
                }
            
                        if ( key ==  263 &&action == GLFW_PRESS) {
                            
                cam.rotateLeft(10);
            }

            if (key == 262 && action == GLFW_PRESS) {
                cam.rotateLeft(-10);
            }

            if (key == 265 && action == GLFW_PRESS) {
                
                cam.rotateUp(10);
            }

            if (key ==265 && action == GLFW_PRESS) {
                cam.rotateUp(-10);
            } },
        }
    );
}