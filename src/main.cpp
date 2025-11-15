#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <sys/types.h>

#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

#include "shader.h"

void handleResizeCallback(GLFWwindow* window, int width, int height);
void handleMouseCallback(GLFWwindow* window, double xPosition,
                         double yPosition);
void handleScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

void processInput(GLFWwindow* window);

constexpr auto VERTEX_PATH = "./shaders/shaders.vert";
constexpr auto FRAGMENT_PATH = "./shaders/shaders.frag";

constexpr uint SCR_WIDTH = 800, SCR_HEIGHT = 600;

constexpr float CAMERA_SPEED = 1.5f;
constexpr float CAMERA_SENSITIVITY = 0.1f;

bool isFirstMouse = true;
float fov = 45.0f, yaw = -90.0f, pitch = 0.0f;
float deltaTime = 0.0f, lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;

glm::vec3 cameraPosition = {0.0f, 0.0f, 2.0f};
glm::vec3 cameraFront = {0.0f, 0.0f, -1.0f};
glm::vec3 cameraUp = {0.0f, 1.0f, 0.0f};

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Linterra", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Failed GLFW window creation");
  }

  glfwMakeContextCurrent(window);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, handleMouseCallback);
  glfwSetScrollCallback(window, handleScrollCallback);

  glfwSetFramebufferSizeCallback(window, handleResizeCallback);
  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("Failed to initialize GLEW");
  }

  glEnable(GL_DEPTH_TEST);

  Shader shader(VERTEX_PATH, FRAGMENT_PATH);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

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

  int uModel = glGetUniformLocation(shader.getId(), "uModel");
  int uView = glGetUniformLocation(shader.getId(), "uView");
  int uProjection = glGetUniformLocation(shader.getId(), "uProjection");

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view =
        glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(
        glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    glUseProgram(shader.getId());  // Use the defined shaders in this program
    glBindVertexArray(VAO);        // Get ready to draw with this VAO

    glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
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
  float xOffset = xPosition - lastX;
  float yOffset = lastY - yPosition;

  lastX = xPosition;
  lastY = yPosition;

  xOffset *= CAMERA_SENSITIVITY;
  yOffset *= CAMERA_SENSITIVITY;

  yaw += xOffset;
  pitch += yOffset;

  if (pitch > 89.0f) {
    pitch = 89.0f;
  }
  if (pitch < -89.0f) {
    pitch = -89.0f;
  }

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);
}

void handleScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
  fov -= (float)yOffset;
  if (fov < 1.0f) {
    fov = 1.0f;
  }
  if (fov > 45.0f) {
    fov = 45.0f;
  }
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    return;
  }

  float cameraSpeed = CAMERA_SPEED * deltaTime;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    cameraPosition += cameraSpeed * cameraFront;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    cameraPosition -= cameraSpeed * cameraFront;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    cameraPosition -=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    cameraPosition +=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    cameraPosition[1] += cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    cameraPosition[1] -= cameraSpeed;
  }
}
