#include "manager.h"
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <print>
#include <stdexcept>

#include "camera.h"
#include "config.h"
#include "shader.h"

void handleResizeCallback(GLFWwindow* window, int width, int height);
void handleMouseCallback(GLFWwindow* window, double xPosition,
                         double yPosition);
void handleScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

void processInput(GLFWwindow* window);

Camera camera({8, 1, 20});

float deltaTime = 0.0f, lastFrame = 0.0f;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window =
      glfwCreateWindow(Constants::SCR_WIDTH, Constants::SCR_HEIGHT, "Linterra",
                       nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Failed GLFW window creation");
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);  // Disable V-Sync
  glEnable(GL_DEPTH_TEST);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, handleMouseCallback);
  glfwSetScrollCallback(window, handleScrollCallback);

  glfwSetFramebufferSizeCallback(window, handleResizeCallback);
  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("Failed to initialize GLEW");
  }

  glClearColor(Constants::BG_COLOR[0], Constants::BG_COLOR[1],
               Constants::BG_COLOR[2], Constants::BG_COLOR[3]);

  Shader shader(Constants::VERTEX_PATH, Constants::FRAGMENT_PATH);
  ChunkManager chunkManager(shader);
  chunkManager.draw();

  

  int uView = glGetUniformLocation(shader.getId(), "uView");
  int uProjection = glGetUniformLocation(shader.getId(), "uProjection");

  uint attempts = 0;
  float combinedDeltaTime = 0;

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    combinedDeltaTime += deltaTime;
    attempts++;
    constexpr uint MAX_ATTEMPTS = 1000;
    if (attempts > MAX_ATTEMPTS) {
      std::println("FPS: {}",
                   std::to_string(1 / (combinedDeltaTime / MAX_ATTEMPTS)));
      combinedDeltaTime = 0;
      attempts = 0;
    }

    processInput(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.getView();
    glm::mat4 projection = camera.getProjection();

    chunkManager.render();
    glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

    glfwSwapBuffers(window);  // Swap the buffer
    glfwPollEvents();         // Get any events if avaialbe

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
      std::string error = (const char*)glewGetErrorString(err);
      throw std::runtime_error("OpenGL Error: " + error);
    }
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}

void handleResizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void handleMouseCallback(GLFWwindow* window, double xPosition,
                         double yPosition) {
  camera.processMouseInput(window, xPosition, yPosition);
}

void handleScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
  camera.processScrollInput(window, xOffset, yOffset);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    return;
  }

  camera.processKeyInput(window, deltaTime);
}
