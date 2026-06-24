#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char *read_shader_file(const char *filepath) {
  // 1. Open the file in binary mode to prevent newline conversions
  FILE *file = fopen(filepath, "rb");
  if (file == NULL) {
    fprintf(stderr, "Failed to open shader file: %s\n", filepath);
    return NULL;
  }

  // 2. Seek to the end to determine file size
  fseek(file, 0L, SEEK_END);
  long size = ftell(file);
  rewind(file);

  // 3. Allocate a buffer large enough for file contents plus a null terminator
  char *buffer = (char *)malloc(size + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Memory allocation failed for shader: %s\n", filepath);
    fclose(file);
    return NULL;
  }

  // 4. Read the file data into the allocated buffer
  size_t bytes_read = fread(buffer, 1, size, file);
  if (bytes_read != size) {
    fprintf(stderr, "Warning: Expected to read %ld bytes, but read %zu\n", size,
            bytes_read);
  }

  // 5. Append the mandatory null terminator
  buffer[bytes_read] = '\0';

  // 6. Clean up the file stream and return the buffer
  fclose(file);
  return buffer;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  printf("apple platform!\n");
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(800, 600, "hello, gl", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    return -1;
  }

  // glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  const char *vertexShaderSource = read_shader_file("./shaders/vertex.glsl");
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 2, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", infoLog);
    exit(69);
  }

  const char *fragmentShaderSource = read_shader_file("./shaders/frag.glsl");
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
  }

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::COMPILATION_FAILED: %s\n", infoLog);
    exit(69);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float vertices[] = {
      // position          // color
      -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,

      +0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,

      -0.0f, +1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  };

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint
  // size, GLenum type, GLboolean normalized, GLsizei stride, const void
  // *pointer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  int prog = shaderProgram;
  glUseProgram(prog);
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float time = glfwGetTime();
    float green = sin(time) / 2.0f + 0.5f;
    printf("current time: %f, green: %f\n", time, green);

    int vColorLoc = glGetUniformLocation(prog, "vertexColor");
    glUniform4f(vColorLoc, 0.0f, green, 0.0f, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  return 0;
}
