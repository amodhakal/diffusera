#include "application.h"

#include <glm/gtc/type_ptr.hpp>
#include <print>
#include <string>

#include "config.h"
#include "player.h"
#include "shader.h"

Application::Application(const char* title, const uint width, const uint height,
                         glm::vec4 bgColor)
    : m_Shader(),
      m_ChunkManager(),
      m_Player(Constants::Camera::DEFAULT_POSITION),
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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
  }

  m_Shader.load(Constants::VERTEX_PATH, Constants::FRAGMENT_PATH);
  // Load block textures and bind to texture units
  try {
    m_Textures[0].loadFromFile("resources/blocks/grass_top.png");
    m_Textures[1].loadFromFile("resources/blocks/grass_top.png");
    m_Textures[2].loadFromFile("resources/blocks/dirt.png");
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Failed to load textures: ") +
                             e.what());
  }

  // Bind textures to units (3 textures)
  for (int i = 0; i < 3; ++i) {
    m_Textures[i].bindToUnit(i);
  }

  m_ChunkManager.load();

  glEnable(GL_DEPTH_TEST);
  glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);

  m_Shader.newUniform("uModel");
  m_Shader.newUniform("uView");
  m_Shader.newUniform("uProjection");
  m_Shader.newUniform("uTextures");

  // set sampler units
  m_Shader.use();
  int texUnits[3] = {0, 1, 2};
  m_Shader.setUniformIntArray("uTextures", texUnits, 3);
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

  glm::mat4 view = m_Player.getView();
  glm::mat4 projection = m_Player.getProjection();

  m_Shader.setUniformMat4("uView", view);
  m_Shader.setUniformMat4("uProjection", projection);

  auto cameraPtr = m_Player.getCamera();
  m_ChunkManager.render(cameraPtr, m_Shader);

  glfwSwapBuffers(m_Window);
  glfwPollEvents();

  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    std::println("OpenGL Error: {}", err);
  }

  glm::vec3 cameraPosition = m_Player.getCamera()->m_Position;
  float highestY = m_ChunkManager.getPositionHighestY(cameraPosition);
  m_Player.update(deltaTime, highestY);
}

float Application::getDeltaTime() {
  float currentFrame = glfwGetTime();
  float deltaTime = currentFrame - m_lastFrame;
  m_lastFrame = currentFrame;

  m_CombinedDeltaTime += deltaTime;
  m_FpsAttempts++;

  constexpr uint MAX_ATTEMPTS = 100;
  if (m_FpsAttempts >= MAX_ATTEMPTS) {
    std::println("FPS: {}",
                 std::to_string(1 / (m_CombinedDeltaTime / MAX_ATTEMPTS)));
    m_CombinedDeltaTime = 0;
    m_FpsAttempts = 0;
  }

  return deltaTime;
}

void Application::processMouseInput(double xPosition, double yPosition) {
  return m_Player.processMouseInput(xPosition, yPosition);
}

void Application::processScrollInput(double xOffset, double yOffset) {
  // TODO No fov change here
  // return m_Player.getCamera()->processScrollInput(xOffset, yOffset);
}

void Application::handleKeyPress(float deltaTime) {
  if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(m_Window, true);
    return;
  }

  m_Player.processKeyInput(m_Window, deltaTime);
}

void Application::handleResizeCallback(GLFWwindow* window, int width,
                                       int height) {
  glViewport(0, 0, width, height);
}

void Application::handleMouseCallback(GLFWwindow* window, double xPosition,
                                      double yPosition) {
  auto* context = static_cast<Application*>(glfwGetWindowUserPointer(window));
  context->processMouseInput(xPosition, yPosition);
}

void Application::handleScrollCallback(GLFWwindow* window, double xOffset,
                                       double yOffset) {
  auto* context = static_cast<Application*>(glfwGetWindowUserPointer(window));
  context->processScrollInput(xOffset, yOffset);
}
