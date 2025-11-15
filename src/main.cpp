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

Camera camera({0, 0, 2});

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
  // glEnable(GL_DEPTH_TEST);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, handleMouseCallback);
  glfwSetScrollCallback(window, handleScrollCallback);

  glfwSetFramebufferSizeCallback(window, handleResizeCallback);
  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("Failed to initialize GLEW");
  }



  Shader shader(Constants::VERTEX_PATH, Constants::FRAGMENT_PATH);

  glClearColor(Constants::BG_COLOR[0], Constants::BG_COLOR[1],
               Constants::BG_COLOR[2], Constants::BG_COLOR[3]);

  float vertices[] = {
      // First rectangle centered at x = 0.5
      0.625f, 0.125f, 0.0f, 0.0f, 0.0f,
      1.0f,  // top right
      0.625f, -0.125f, 0.0f, 0.0f, 1.0f,
      0.0f,  // bottom right
      0.375f, -0.125f, 0.0f, 0.0f, 1.0f,
      0.0f,  // bottom left
      0.375f, 0.125f, 0.0f, 0.0f, 0.0f,
      1.0f,  // top left

      // Second rectangle centered at x = -0.5
      -0.375f, 0.125f, 0.0f, 1.0f, 0.0f,
      0.0f,  // top right
      -0.375f, -0.125f, 0.0f, 0.0f, 1.0f,
      0.0f,  // bottom right
      -0.625f, -0.125f, 0.0f, 0.0f, 1.0f,
      0.0f,  // bottom left
      -0.625f, 0.125f, 0.0f, 1.0f, 0.0f,
      0.0f  // top left
  };

  uint indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3,  // second triangle
      4, 5, 7,  // third triangle
      5, 6, 7   // forth triangle
  };

  // Used to manage everything (VBO, EBO) in 1 place
  uint VAO;
  glGenVertexArrays(1, &VAO);

  // Start the configuration
  glBindVertexArray(VAO);

  // Used to store vertex info to be sent to the gpu
  uint VBO;
  glGenBuffers(1, &VBO);               // Creates a buffer on the gpu
  glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Binds to VAO
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW);  // Copy info from CPU to GPU

  // Configure info (vertex) for location 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Configure info (color) for location 1
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Used to store the indices to setup the vertices to create triangle
  uint EBO;
  glGenBuffers(1, &EBO);                       // Creates a buffer on the gpu
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  // Binds to VAO
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);  // Copy info from CPU to GPU

  // Stop the configuration

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
    if (attempts > 100) {
      std::println("FPS: {}", std::to_string(1 / (combinedDeltaTime / 100)));
      combinedDeltaTime = 0;
      attempts = 0;
    }

    processInput(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.getView();
    glm::mat4 projection = camera.getProjection();

    glUseProgram(shader.getId());  // Use the defined shaders in this program
    glBindVertexArray(VAO);        // Get ready to draw with this VAO

    glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT,
                   0);  // Draw the objects

    glfwSwapBuffers(window);  // Swap the buffer
    glfwPollEvents();         // Get any events if avaialbe

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
      throw std::runtime_error(&"GL Error: "[err]);
    }
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shader.getId());

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
