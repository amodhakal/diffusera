#pragma once
#include <glm/glm.hpp>
#include <print>

#include "camera.h"
#include "config.h"

class Player {
 public:
  Player(const glm::vec3& position);
  void update(float deltaTime, int currentY);
  void jump(float cameraSpeed);

  Camera* getCamera();

  glm::mat4 getView();
  glm::mat4 getProjection();

  void processKeyInput(GLFWwindow* window, float deltaTime);
  void processMouseInput(double xPosition, double yPosition);

 private:
  glm::vec3 m_Velocity;
  Camera m_Camera;

  bool m_AllowJumping;
};

inline Player::Player(const glm::vec3& position)
    : m_Camera(position), m_Velocity(0, 0, 0), m_AllowJumping(false) {
}

inline void Player::update(float deltaTime, int currentY) {
  if (Constants::DO_GRAVITY) {
    // Handle gravity
    m_Velocity.y += -Constants::Camera::ACCELERATION * deltaTime;
    if (m_Velocity.y < 0 && m_Velocity.y < -Constants::Camera::MAX_VELOCITY) {
      m_Velocity.y = -Constants::Camera::MAX_VELOCITY;
    }

    m_Camera.m_Position.y += m_Velocity.y * deltaTime;

    if (m_Camera.m_Position.y <= currentY + 2) {
      m_Camera.m_Position.y = currentY + 2;
      m_Velocity.y = 0;
      m_AllowJumping = true;
    } else {
      m_AllowJumping = false;
    }
  }
}

inline void Player::jump(float cameraSpeed) {
  if (m_AllowJumping && Constants::DO_GRAVITY) {
    m_Velocity.y = Constants::Camera::JUMP_VELOCTY;
    m_AllowJumping = true;
  } else {
    m_Camera.m_Position.y += cameraSpeed;
  }
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

inline void Player::processKeyInput(GLFWwindow* window, float deltaTime) {
  float cameraSpeed = Constants::Camera::SPEED * deltaTime;
  float previousY = m_Camera.m_Position.y;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    m_Camera.m_Position += cameraSpeed * m_Camera.m_Front;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    m_Camera.m_Position -= cameraSpeed * m_Camera.m_Front;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    m_Camera.m_Position -=
        glm::normalize(glm::cross(m_Camera.m_Front, m_Camera.m_Up)) *
        cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    m_Camera.m_Position +=
        glm::normalize(glm::cross(m_Camera.m_Front, m_Camera.m_Up)) *
        cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    jump(cameraSpeed);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    if (!Constants::DO_GRAVITY) {
      m_Camera.m_Position[1] -= cameraSpeed;
    }
  }

  if (Constants::DO_GRAVITY) {
    // Don't change y if gravity is enabled
    m_Camera.m_Position.y = previousY;
  }
}

inline void Player::processMouseInput(double xPosition, double yPosition) {
  float xOffset = xPosition - m_Camera.m_LastX;
  float yOffset = m_Camera.m_LastY - yPosition;

  m_Camera.m_LastX = xPosition;
  m_Camera.m_LastY = yPosition;

  xOffset *= Constants::Camera::SENSITIVITY;
  yOffset *= Constants::Camera::SENSITIVITY;

  m_Camera.m_Yaw += xOffset;
  m_Camera.m_Pitch += yOffset;

  m_Camera.m_Pitch = fmax(m_Camera.m_Pitch, Constants::Camera::PITCH_MIN);
  m_Camera.m_Pitch = fmin(m_Camera.m_Pitch, Constants::Camera::PITCH_MAX);

  glm::vec3 front;
  front.x =
      cos(glm::radians(m_Camera.m_Yaw)) * cos(glm::radians(m_Camera.m_Pitch));
  front.y = sin(glm::radians(m_Camera.m_Pitch));
  front.z =
      sin(glm::radians(m_Camera.m_Yaw)) * cos(glm::radians(m_Camera.m_Pitch));

  m_Camera.m_Front = glm::normalize(front);
}
