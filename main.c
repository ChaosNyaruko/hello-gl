#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "layout (location = 1) in vec3 aColor;\n"
      "out vec3 ourColor;\n" 
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos, 1.0);\n"
      "   ourColor = aColor;\n"
      "}\0";
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

  const char *fragmentShaderSourceOrange =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "in vec3 ourColor;\n"
      "void main()\n"
      "{\n"
      "  FragColor = vec4(ourColor, 1.0);\n"
      "}\0";

  unsigned int fragmentShaderOrange;
  fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSourceOrange, NULL);
  glCompileShader(fragmentShaderOrange);
  glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShaderOrange, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
  }

  unsigned int shaderProgramOrange;
  shaderProgramOrange = glCreateProgram();
  glAttachShader(shaderProgramOrange, vertexShader);
  glAttachShader(shaderProgramOrange, fragmentShaderOrange);
  glLinkProgram(shaderProgramOrange);
  glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgramOrange, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::COMPILATION_FAILED: %s\n", infoLog);
    exit(69);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShaderOrange);

  float t2[] = { 
      // position          // color
      -0.5f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
      +0.5f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
      -0.0f, +1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
  };

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(t2), t2, GL_STATIC_DRAW);
  // typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  int prog = shaderProgramOrange;
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
  glDeleteProgram(shaderProgramOrange);
  glfwTerminate();
  return 0;
}
