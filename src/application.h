#pragma once

#include <glad/glad.h>

#include <print>

#include "camera.h"
#include "config.h"
#include "manager.h"
#include "shader.h"

class Application {
 public:
  Application(const char* title, const uint width = Constants::SCR_WIDTH,
              const uint height = Constants::SCR_HEIGHT,
              glm::vec4 bgColor = Constants::BG_COLOR);
  ~Application();

  bool isRunning();
  void update();

  Camera* getCamera();

 private:
  GLFWwindow* m_Window;
  Shader m_Shader;
  ChunkManager m_ChunkManager;
  Camera m_Camera;

  uint m_FpsAttempts;
  float m_CombinedDeltaTime;
  float m_lastFrame;

  float getDeltaTime();

  void handleKeyPress(float deltaTime);
  static void handleResizeCallback(GLFWwindow* window, int width, int height);
  static void handleMouseCallback(GLFWwindow* window, double xPosition,
                                  double yPosition);
  static void handleScrollCallback(GLFWwindow* window, double xOffset,
                                   double yOffset);
};
