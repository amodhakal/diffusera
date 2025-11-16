#include "config.h"
#include "camera.h"

Camera::Camera(glm::vec3 position) {
  m_Position = position;
  m_Front = Constants::Camera::DEFAULT_FRONT;
  m_Up = Constants::Camera::DEFAULT_UP;

  m_Fov = Constants::Camera::DEFAULT_FOV;
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

void Camera::processKeyInput(GLFWwindow *window, float deltaTime) {
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
  float aspect = (float)Constants::SCR_WIDTH / (float)Constants::SCR_HEIGHT;
  return glm::perspective(glm::radians(m_Fov), aspect, Constants::Camera::NEAR,
                          Constants::Camera::FAR);
}
