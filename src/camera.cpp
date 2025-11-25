#include "camera.h"

#include <cmath>
#include <print>

#include "chunk.h"
#include "config.h"

Camera::Camera(glm::vec3 position) {
  m_Position = position;
  m_Front = Constants::Camera::DEFAULT_FRONT;
  m_Up = Constants::Camera::DEFAULT_UP;
  m_WorldUp = m_Up;

  m_Near = Constants::Camera::NEAR;
  m_Far = Constants::Camera::FAR;

  m_Fov = Constants::Camera::DEFAULT_FOV;
  m_Aspect = (float)Constants::SCR_WIDTH / (float)Constants::SCR_HEIGHT;

  m_Yaw = Constants::Camera::DEFAULT_YAW;
  m_Pitch = Constants::Camera::DEAULT_PITCH;

  m_LastX = Constants::SCR_WIDTH / 2.0;
  m_LastY = Constants::SCR_HEIGHT / 2.0;

  m_IsFirstMouse = true;
}

void Camera::processMouseInput(double xPosition, double yPosition) {
  float xOffset = xPosition - m_LastX;
  float yOffset = m_LastY - yPosition;

  m_LastX = xPosition;
  m_LastY = yPosition;

  xOffset *= Constants::Camera::SENSITIVITY;
  yOffset *= Constants::Camera::SENSITIVITY;

  m_Yaw += xOffset;
  m_Pitch += yOffset;

  m_Pitch = fmax(m_Pitch, Constants::Camera::PITCH_MIN);
  m_Pitch = fmin(m_Pitch, Constants::Camera::PITCH_MAX);

  glm::vec3 front;
  front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  front.y = sin(glm::radians(m_Pitch));
  front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  m_Front = glm::normalize(front);
}

void Camera::processScrollInput(double xOffset, double yOffset) {
  m_Fov -= yOffset;
  m_Fov = fmax(m_Fov, Constants::Camera::FOV_MIN);
  m_Fov = fmin(m_Fov, Constants::Camera::FOV_MAX);
}

void Camera::processKeyInput(GLFWwindow* window, float deltaTime) {
  float cameraSpeed = Constants::Camera::SPEED * deltaTime;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    m_Position += cameraSpeed * m_Front;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    m_Position -= cameraSpeed * m_Front;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    m_Position[1] += cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    m_Position[1] -= cameraSpeed;
  }
}

glm::mat4 Camera::getView() {
  return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::getProjection() {
  return glm::perspective(glm::radians(m_Fov), m_Aspect,
                          Constants::Camera::NEAR, Constants::Camera::FAR);
}

glm::vec3 Camera::getRight() const {
  return glm::normalize(glm::cross(m_Up, m_WorldUp));
}

bool Camera::isChunkInside(const glm::vec2& position) const {
  // TODO: THe frustum setup shouldn't be repeated
  // Frustum initialization
  float halfVSide = m_Far * tan(m_Fov * 0.5);
  float halfHSide = halfVSide * m_Aspect;
  const glm::vec3 frontMultFar = m_Far * m_Front;

  Plane nearFace = {m_Position + m_Near * m_Front, m_Front};
  Plane farFace = {m_Position + frontMultFar, -m_Front};
  Plane rightFace = {m_Position,
                     glm::cross(frontMultFar - getRight() * halfHSide, m_Up)};
  Plane leftFace = {m_Position,
                    glm::cross(m_Up, frontMultFar + getRight() * halfHSide)};
  Plane topFace = {m_Position,
                   glm::cross(getRight(), frontMultFar - m_Up * halfVSide)};
  Plane bottomFace = {m_Position,
                      glm::cross(frontMultFar + m_Up * halfVSide, getRight())};

  // Frustum checking

  glm::vec3 chunkCorner1 = {
      static_cast<float>((position.s * Constants::Chunk::LENGTH) -
                         (Constants::Chunk::LENGTH / 2.0)),
      0.0,
      static_cast<float>((position.t * Constants::Chunk::LENGTH) -
                         (Constants::Chunk::LENGTH / 2.0))};
  glm::vec3 chunkCorner2 = {chunkCorner1.x + Constants::Chunk::LENGTH,
                            Constants::Chunk::HEIGHT,
                            chunkCorner1.z + Constants::Chunk::LENGTH};

  glm::vec3 corners[8];
  int i = 0;
  for (int xi = 0; xi < 2; ++xi) {
    for (int yi = 0; yi < 2; ++yi) {
      for (int zi = 0; zi < 2; ++zi) {
        corners[i++] = glm::vec3(xi ? chunkCorner2.x : chunkCorner1.x,
                                 yi ? chunkCorner2.y : chunkCorner1.y,
                                 zi ? chunkCorner2.z : chunkCorner1.z);
      }
    }
  }

  const Plane planes[6] = {topFace,  bottomFace, rightFace,
                           leftFace, farFace,    nearFace};

  for (const Plane& p : planes) {
    int outsideCount = 0;
    for (const glm::vec3& c : corners) {
      float distance = glm::dot(c - p.point, p.normal);
      if (distance < 0.0f) {
        ++outsideCount;
      }
    }
    if (outsideCount == 8) {
      return false;
    }
  }

  return true;
}
