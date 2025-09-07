#include "shaders.h"
#include <glad.h>

#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
}

GLFWwindow *window_setup() {
  glfwInit();
  // Set opengl versions
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Needed on macos
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(window);

  // Init GLAD to load the address of the OpenGL function pointers (OS Specific)
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialise GLAD\n");
    return NULL;
  }

  // Set rendering viewport and callback for resizing
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  return window;
}

int main(int argc, char *argv[]) {
  GLFWwindow *window = window_setup();
  if (window == NULL) {
    return -1;
  }

  // Set up shaders
  unsigned int shaderProgram =
      shader_create("res/vertex.vs", "res/fragment.fs");

  float vertices[] = {
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, // top
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0,  // bottom left
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f  // bottom right
  };

  // Setup
  unsigned int VAOs[2], VBOs[2];
  glGenVertexArrays(1, VAOs);
  glGenBuffers(1, VBOs);

  // Triangle
  glBindVertexArray(VAOs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Postion data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Colour data
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    // This sets a value in the sate machine
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // This uses the value
    glClear(GL_COLOR_BUFFER_BIT);

    shader_use(shaderProgram);

    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}
