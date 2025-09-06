#include <glad.h>

#include <GLFW/glfw3.h>
#include <stdio.h>

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
}

int main(int argc, char *argv[]) {

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
    return -1;
  }

  glfwMakeContextCurrent(window);

  // Init GLAD to load the address of the OpenGL function pointers (OS Specific)
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialise GLAD\n");
    return -1;
  }

  // Set rendering viewport and callback for resizing
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // This sets a value in the sate machine
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // This uses the value
    glClear(GL_COLOR_BUFFER_BIT);

    // rendering
    float vertices[] = {
        // Left
        -0.5f,
        -0.5f,
        0.0f,
        // Right
        0.5f,
        -0.5f,
        0.0f,
        // Top
        0.0f,
        0.5f,
        0.0f,
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    unsigned int VBO; // Verex buffer object, place to store vertices on GPU
    glGenBuffers(1, &VBO);
    // OpenGL can bind several buffers at once if they are different types
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy the data into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set up shaders
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s\n", infoLog);
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    if (!success) {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s\n", infoLog);
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      printf("ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n %s\n", infoLog);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    // These are not needed after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
