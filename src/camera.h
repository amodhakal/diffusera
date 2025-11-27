#pragma once
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Plane {
  glm::vec3 point = {0.0f, 0.0f, 0.0f};
  glm::vec3 normal = {0.0f, 1.0f, 0.0f};
};

struct Camera {
  glm::vec3 m_Position, m_Front, m_Up, m_WorldUp;
  float m_Near, m_Far;

  float m_Aspect, m_Fov;
  float m_Yaw, m_Pitch;
  float m_LastX, m_LastY;
  bool m_IsFirstMouse;

  Camera(glm::vec3 position);
  void processMouseInput(double xPosition, double yPosition);
  void processScrollInput(double xOffset, double yOffset);
  void processKeyInput(GLFWwindow* window, float deltaTime);

  glm::mat4 getView();
  glm::mat4 getProjection();
  glm::vec3 getRight() const;

  bool isChunkInside(const glm::vec2& position) const;
};
