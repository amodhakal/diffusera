#pragma once
#include <glm/glm.hpp>

#include "camera.h"

class Player {
 public:
  Player(const glm::vec3& position);
  Camera* getCamera();
  glm::mat4 getView();
  glm::mat4 getProjection();

 private:
  Camera m_Camera;
};

inline Player::Player(const glm::vec3& position) : m_Camera(position) {
}

inline Camera* Player::getCamera() {
  return &m_Camera;
}

inline glm::mat4 Player::getView() {
  return m_Camera.getView();
}

inline glm::mat4 Player::getProjection() {
  return m_Camera.getProjection();
}
