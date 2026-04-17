#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include <cmath>
class FreeflyCamera {
private:
  glm::vec3 m_Position;
  float m_fPhi;
  float m_fTheta;

  glm::vec3 m_FrontVector;
  glm::vec3 m_LeftVector;
  glm::vec3 m_UpVector;

  void computeDirectionVectors() {
    m_FrontVector = glm::vec3(
        std::cos(glm::radians(m_fTheta)) * std::sin(glm::radians(m_fPhi)),
        std::sin(glm::radians(m_fTheta)),
        std::cos(glm::radians(m_fTheta)) * std::cos(glm::radians(m_fPhi)));
    m_LeftVector =
        glm::vec3(std::sin(glm::radians(m_fPhi) + (glm::pi<float>() / 2.f)), 0,
                  std::cos(glm::radians(m_fPhi) + (glm::pi<float>() / 2.f)));
    m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
  };

public:
  // constructor
  FreeflyCamera()
      : m_Position(glm::vec3(0)), m_fPhi(glm::pi<float>()), m_fTheta(0) {
    computeDirectionVectors();
  };

  // camera mouvement
  void moveLeft(float t) { m_Position += t * m_LeftVector; };
  void moveFront(float t) { m_Position += t * m_FrontVector; };

  void rotateLeft(float degrees) { m_fPhi -= degrees; };
  void rotateUp(float degrees) { m_fTheta += degrees; };

  glm::mat4 getViewMaxtrix() const {
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
  }
};