#include <glm/gtc/type_ptr.hpp>
#include "application.h"

Application::Application(const char* title, const uint width, const uint height,
                         glm::vec4 bgColor)
    : m_Shader(),
      m_ChunkManager(),
      m_Camera(Constants::Camera::DEFAULT_POSITION),
      m_FpsAttempts(0),
      m_CombinedDeltaTime(0),
      m_lastFrame(0)

{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!m_Window) {
    throw std::runtime_error("Failed to create window");
  }

  glfwMakeContextCurrent(m_Window);
  glfwSwapInterval(0);  // Disable V-Sync

  glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(m_Window, handleMouseCallback);
  glfwSetScrollCallback(m_Window, handleScrollCallback);
  glfwSetFramebufferSizeCallback(m_Window, handleResizeCallback);
  glfwSetWindowUserPointer(m_Window, this);

  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("Failed to initialize GLEW");
  }

  m_Shader.load(Constants::VERTEX_PATH, Constants::FRAGMENT_PATH);
  m_ChunkManager.load(m_Shader);

  glEnable(GL_DEPTH_TEST);
  glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);

  m_Uniforms["uView"] = glGetUniformLocation(m_Shader.getId(), "uView");
  m_Uniforms["uProjection"] =
      glGetUniformLocation(m_Shader.getId(), "uProjection");
}

Application::~Application() {
  glfwTerminate();
}

bool Application::isRunning() {
  return !glfwWindowShouldClose(m_Window);
}

void Application::update() {
  float deltaTime = getDeltaTime();
  handleKeyPress(deltaTime);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TODO Optimize this
  m_ChunkManager.render();

  glm::mat4 view = m_Camera.getView();
  glm::mat4 projection = m_Camera.getProjection();
  glUniformMatrix4fv(m_Uniforms["uView"], 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(m_Uniforms["uProjection"], 1, GL_FALSE,
                     glm::value_ptr(projection));

  glfwSwapBuffers(m_Window);
  glfwPollEvents();

  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    std::string error = (const char*)glewGetErrorString(err);
    throw std::runtime_error("OpenGL Error: " + error);
  }
}

Camera* Application::getCamera() {
  return &this->m_Camera;
}

float Application::getDeltaTime() {
  float currentFrame = glfwGetTime();
  float deltaTime = currentFrame - m_lastFrame;
  m_lastFrame = currentFrame;

  m_CombinedDeltaTime += deltaTime;
  m_FpsAttempts++;

  constexpr uint MAX_ATTEMPTS = 1000;
  if (m_FpsAttempts >= MAX_ATTEMPTS) {
    std::println("FPS: {}",
                 std::to_string(1 / (m_CombinedDeltaTime / MAX_ATTEMPTS)));
    m_CombinedDeltaTime = 0;
    m_FpsAttempts = 0;
  }

  return deltaTime;
}

void Application::handleKeyPress(float deltaTime) {
  if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(m_Window, true);
    return;
  }

  m_Camera.processKeyInput(m_Window, deltaTime);
}

void Application::handleResizeCallback(GLFWwindow* window, int width,
                                       int height) {
  glViewport(0, 0, width, height);
}

void Application::handleMouseCallback(GLFWwindow* window, double xPosition,
                                      double yPosition) {
  auto* context = static_cast<Application*>(glfwGetWindowUserPointer(window));
  context->getCamera()->processMouseInput(xPosition, yPosition);
}

void Application::handleScrollCallback(GLFWwindow* window, double xOffset,
                                       double yOffset) {
  auto* context = static_cast<Application*>(glfwGetWindowUserPointer(window));
  context->getCamera()->processScrollInput(xOffset, yOffset);
}
