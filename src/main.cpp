#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>

void handleResizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
const std::string getFullFileContents(const char *filePath);

constexpr auto VERTEX_PATH = "./shaders/shaders.vert";
constexpr auto FRAGMENT_PATH = "./shaders/shaders.frag";

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Linterra", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Failed GLFW window creation");
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, handleResizeCallback);
  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("Failed to initialize GLEW");
  }

  const std::string vertexShaderCodeRaw = getFullFileContents(VERTEX_PATH);
  const std::string fragmentShaderCodeRaw = getFullFileContents(FRAGMENT_PATH);
  const char *vertexShaderCode = vertexShaderCodeRaw.c_str();
  const char *fragmentShaderCode = fragmentShaderCodeRaw.c_str();

  uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
  glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);

  glCompileShader(vertexShader);
  glCompileShader(fragmentShader);

  int success;
  char infoLog[512];

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    throw std::runtime_error("Vertex compilation: " + std::string(infoLog));
  }

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    throw std::runtime_error("Fragment compilation: " + std::string(infoLog));
  }

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    throw std::runtime_error("Shader linking: " + std::string(infoLog));
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  float vertices[] = {
      // First rectangle centered at x = 0.5
      0.625f, 0.125f, 0.0f, 0.0f, 0.0f, 1.0f,  // top right
      0.625f, -0.125f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
      0.375f, -0.125f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
      0.375f, 0.125f, 0.0f, 0.0f, 0.0f, 1.0f,  // top left

      // Second rectangle centered at x = -0.5
      -0.375f, 0.125f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
      -0.375f, -0.125f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.625f, -0.125f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
      -0.625f, 0.125f, 0.0f, 1.0f, 0.0f, 0.0f   // top left
  };

  uint indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3, // second triangle
      4, 5, 7, // third triangle
      5, 6, 7  // forth triangle
  };

  // Used to manage everything (VBO, EBO) in 1 place
  uint VAO;
  glGenVertexArrays(1, &VAO);

  // Start the configuration
  glBindVertexArray(VAO);

  // Used to store vertex info to be sent to the gpu
  uint VBO;
  glGenBuffers(1, &VBO);              // Creates a buffer on the gpu
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binds to VAO
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW); // Copy info from CPU to GPU

  // Configure info (vertex) for location 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Configure info (color) for location 1
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Used to store the indices to setup the vertices to create triangle
  uint EBO;
  glGenBuffers(1, &EBO);                      // Creates a buffer on the gpu
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Binds to VAO
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW); // Copy info from CPU to GPU

  // Stop the configuration

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram); // Use the defined shaders in this program
    glBindVertexArray(VAO);      // Get ready to draw with this VAO
    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT,
                   0); // Draw the objects

    glfwSwapBuffers(window); // Swap the buffer
    glfwPollEvents();        // Get any events if avaialbe

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
      throw std::runtime_error(&"GL Error: "[err]);
    }
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return EXIT_SUCCESS;
}

void handleResizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

const std::string getFullFileContents(const char *filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    throw std::runtime_error("Couldn't open file: " + std::string(filePath));
  }

  std::string content, line;
  while (getline(file, line)) {
    content += line + "\n";
  }

  return content;
}
