#define STB_IMAGE_IMPLEMENTATION

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include <glad.h>

#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

#include <cglm/affine-pre.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <cglm/util.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <stb_image.h>

#include "shaders.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
GLFWwindow *window_setup();

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float yaw, pitch;
float lastX = 400, lastY = 300;

vec3 cameraPos = {0.0f, 0.0f, 3.0f};
vec3 cameraFront = {0.0f, 0.0f, -1.0f};
vec3 cameraUp = {0.0f, 1.0f, 0.0f};

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }

  const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    // Equivalent to cameraPos += cameraSpeed * cameraFront;
    // Use glm_vec3_muladds to multiply and add in a single operation
    glm_vec3_muladds(cameraFront, cameraSpeed, cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    // Equivalent to cameraPos -= cameraSpeed * cameraFront;
    vec3 temp;
    glm_vec3_scale(cameraFront, cameraSpeed, temp);
    glm_vec3_sub(cameraPos, temp, cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    // Equivalent to cameraPos -= glm::normalize(glm::cross(cameraFront,
    // cameraUp)) * cameraSpeed;
    vec3 right;
    vec3 normalized_right;

    // Calculate the cross product
    glm_vec3_cross(cameraFront, cameraUp, right);

    // Normalize the resulting vector
    glm_vec3_normalize(right);

    // Scale and subtract from cameraPos
    glm_vec3_scale(right, cameraSpeed, normalized_right);
    glm_vec3_sub(cameraPos, normalized_right, cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    // Equivalent to cameraPos += glm::normalize(glm::cross(cameraFront,
    // cameraUp)) * cameraSpeed;
    vec3 right;
    vec3 normalized_right;

    // Calculate the cross product
    glm_vec3_cross(cameraFront, cameraUp, right);

    // Normalize the resulting vector
    glm_vec3_normalize(right);

    // Scale and add to cameraPos
    glm_vec3_scale(right, cameraSpeed, normalized_right);
    glm_vec3_add(cameraPos, normalized_right, cameraPos);
  }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  vec3 direction;
  direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
  direction[1] = sin(glm_rad(pitch));
  direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));

  glm_vec3_normalize_to(direction, cameraFront);
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

  glfwSetCursorPosCallback(window, mouse_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glEnable(GL_DEPTH_TEST);

  return window;
}

int main(int argc, char *argv[]) {
  GLFWwindow *window = window_setup();
  if (window == NULL) {
    return -1;
  }

  // Set up shaders
  unsigned int shaderProgram =
      shader_create("res/shaders/vertex.vs", "res/shaders/fragment.fs");

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Textures
  unsigned int texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("res/assets/textures/container.jpg", &width,
                                  &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("Failed to load texture\n");
  }
  stbi_image_free(data);

  unsigned int texture2;
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("res/assets/textures/awesomeface.png", &width, &height,
                   &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("Failed to load texture\n");
  }
  stbi_image_free(data);

  shader_use(shaderProgram);
  shader_set_int(shaderProgram, "texture1", 0);
  shader_set_int(shaderProgram, "texture2", 1);

  vec3 cubePositions[] = {{0.0f, 0.0f, 0.0f},
                          {2.0f, 5.0f, -15.0f},
                          {-1.5f, -2.2f, -2.5f},
                          {-3.8f, -2.0f, -12.3f},
                          {2.4f, -0.4f, -3.5f},
                          {-1.7f, 3.0f, -7.5f},
                          {1.3f, -2.0f, -2.5f},
                          {1.5f, 2.0f, -2.5f},
                          {1.5f, 0.2f, -1.5f},
                          -1.3f,
                          1.0f,
                          -1.5f};

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    const float radius = 10.0f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;

    mat4 view;
    vec3 center;
    glm_vec3_add(cameraPos, cameraFront, center);
    glm_lookat(cameraPos, center, cameraUp, view);

    mat4 projection;
    glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);

    shader_use(shaderProgram);

    unsigned int viewLocation = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, view[0]);

    unsigned int projectionLocation =
        glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection[0]);

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++) {
      mat4 model;
      glm_mat4_identity(model);
      glm_translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});

      unsigned int modelLocation = glGetUniformLocation(shaderProgram, "model");
      glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model[0]);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}
