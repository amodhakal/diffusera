#pragma once
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "config.h"

class Camera {
 public:
  Camera(glm::vec3 position);
  void processMouseInput(GLFWwindow *window, double xPosition,
                         double yPosition);
  void processScrollInput(GLFWwindow *window, double xOffset, double yOffset);
  void processKeyInput(GLFWwindow *window);

  glm::mat4 getView();
  glm::mat4 getProjection();

 private:
  glm::vec3 m_Position, m_Front, m_Up;
  float m_Fov, m_Yaw, m_Pitch;
  bool m_IsFirstMouse;
};

Camera::Camera(glm::vec3 position) {
  m_Position = position;
  m_Front = Constants::Camera::DEFAULT_FRONT;
  m_Up = Constants::Camera::DEFAULT_UP;

  m_Fov = Constants::Camera::DEFAULT_FOV;
  m_Yaw = Constants::Camera::DEFAULT_YAW;
  m_Pitch = Constants::Camera::DEAULT_PITCH;

  m_IsFirstMouse = true;
}

void Camera::processMouseInput(GLFWwindow *window, double xPosition,
                               double yPosition) {
  //   float xOffset = xPosition - lastX;
  //   float yOffset = lastY - yPosition;

  //   lastX = xPosition;
  //   lastY = yPosition;

  //   xOffset *= Constants::Camera::SENSITIVITY;
  //   yOffset *= Constants::Camera::SENSITIVITY;

  //   yaw += xOffset;
  //   pitch += yOffset;

  //   if (pitch > 89.0f) {
  //     pitch = 89.0f;
  //   }
  //   if (pitch < -89.0f) {
  //     pitch = -89.0f;
  //   }

  //   glm::vec3 front;
  //   front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  //   front.y = sin(glm::radians(pitch));
  //   front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  //   cameraFront = glm::normalize(front);
}

void Camera::processScrollInput(GLFWwindow *window, double xOffset,
                                double yOffset) {
  //   fov -= (float)yOffset;
  //   if (fov < 1.0f) {
  //     fov = 1.0f;
  //   }
  //   if (fov > 45.0f) {
  //     fov = 45.0f;
  //   }
}

void Camera::processKeyInput(GLFWwindow *window) {
  //       float cameraSpeed = Constants::Camera::SPEED * deltaTime;

  //   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
  //     cameraPosition += cameraSpeed * cameraFront;
  //   }
  //   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
  //     cameraPosition -= cameraSpeed * cameraFront;
  //   }
  //   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
  //     cameraPosition -=
  //         glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  //   }
  //   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
  //     cameraPosition +=
  //         glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  //   }
  //   if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
  //     cameraPosition[1] += cameraSpeed;
  //   }
  //   if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
  //     cameraPosition[1] -= cameraSpeed;
  //   }
}

glm::mat4 Camera::getView() {
  return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::getProjection() {
  float aspect = (float)Constants::SCR_WIDTH / (float)Constants::SCR_HEIGHT;
  return glm::perspective(glm::radians(m_Fov), aspect, Constants::Camera::NEAR,
                          Constants::Camera::FAR);
}
