#pragma once
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
 public:
  Camera(glm::vec3 position);
  void processMouseInput(double xPosition, double yPosition);
  void processScrollInput(double xOffset, double yOffset);
  void processKeyInput(GLFWwindow *window, float deltaTime);

  glm::mat4 getView();
  glm::mat4 getProjection();

 private:
  glm::vec3 m_Position, m_Front, m_Up;
  float m_Fov, m_Yaw, m_Pitch;
  float m_LastX, m_LastY;
  bool m_IsFirstMouse;
};
