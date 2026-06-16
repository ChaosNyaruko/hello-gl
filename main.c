#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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
  }

  const char *fragmentShaderSourceOrange =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
  }

  const char *fragmentShaderSourceYellow =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "  FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
      "}\0";

  unsigned int fragmentShaderYellow;
  fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);
  glCompileShader(fragmentShaderYellow);
  glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
  }

  unsigned int shaderProgramYellow;
  shaderProgramYellow = glCreateProgram();
  glAttachShader(shaderProgramYellow, vertexShader);
  glAttachShader(shaderProgramYellow, fragmentShaderYellow);
  glLinkProgram(shaderProgramYellow);
  glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgramYellow, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::COMPILATION_FAILED: %s\n", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShaderYellow);
  glDeleteShader(fragmentShaderOrange);

  float t1[] = {
      -0.1f, 0.0f, 0.0f,  
      -0.5f, 0.0f, 0.0f, 
      -0.1f, 1.0f, 0.0f,   
  };
  float t2[] = {
      -0.1f, 0.0f, 0.0f,  
      -0.5f, 0.0f, 0.0f, 
      -0.1f, -1.0f, 0.0f,   
      // -0.1f, 0.0f, 0.0f,  
      // -0.5f, 0.0f, 0.0f, 
      // -0.1f, 0.5f, 0.0f,   
  };

  unsigned int VAOs[2];
  glGenVertexArrays(2, VAOs);

  unsigned int VBOs[2];
  glGenBuffers(2, VBOs);

  glBindVertexArray(VAOs[0]); 
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(t1), t1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
  glBindVertexArray(0); 

  glBindVertexArray(VAOs[1]); 
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(t2), t2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
  glBindVertexArray(0); 

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgramYellow);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glUseProgram(shaderProgramOrange);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  glDeleteProgram(shaderProgramOrange);
  glDeleteProgram(shaderProgramYellow);
  glfwTerminate();
  return 0;
}
