#pragma once
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
class TrackballCamera {
private:
    float m_fDistance;
    float m_fAngleX;
    float m_fAngleY;

public:
    // constructor
    TrackballCamera() : m_fDistance(5.f), m_fAngleX(0.f), m_fAngleY(0.f) {};

    // camera mouvement
    void moveFront(float delta) { m_fDistance -= delta; };
    void rotateLeft(float degrees) { m_fAngleY -= degrees; };
    void rotateUp(float degrees) { m_fAngleX += degrees; };

    glm::mat4 getViewMaxtrix() const
    {
        glm::mat4 View_Matrix = glm::translate(
            glm::mat4(1), glm::vec3(0, 0, -m_fDistance)
        );                                                                                   // translation
        View_Matrix = glm::rotate(View_Matrix, glm::radians(m_fAngleX), glm::vec3(1, 0, 0)); // rotation around axis X
        View_Matrix = glm::rotate(View_Matrix, glm::radians(m_fAngleY), glm::vec3(0, 1, 0)); // rotation around axis Y
        return View_Matrix;
    };
};