#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/Program.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/common.hpp>

struct BypassBaseProgram {
    glimac::Program m_Program;

    GLuint uMVPMatrix;
    GLuint uMVMatrix;
    GLuint uNormalMatrix;
    GLuint uTexture;

    BypassBaseProgram(const glimac::FilePath& applicationPath, std::string const& vs_file, std::string const& fs_file)
        : m_Program(glimac::loadProgram(applicationPath.dirPath() + vs_file, applicationPath.dirPath() + fs_file))
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture      = glGetUniformLocation(m_Program.getGLId(), "uTexture");
    }
};
