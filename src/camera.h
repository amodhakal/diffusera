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
  const glm::vec3 getPosition() const;
  const glm::vec3 getFront() const;
  const glm::vec3 getUp() const;
  const glm::vec3 getRight() const;

  const float getAspect() const;

 private:
  glm::vec3 m_Position, m_Front, m_Up, m_WorldUp;
  float m_Aspect, m_Fov;
  float m_Yaw, m_Pitch;
  float m_LastX, m_LastY;
  bool m_IsFirstMouse;
};
