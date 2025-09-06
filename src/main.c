#include <glad.h>

#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"
                                 "out vec3 ourColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos, 1.0);\n"
                                 "   ourColor = aColor;\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec3 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(ourColor, 1.0);\n"
                                   "}\0";

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
}

void setup_shader(unsigned int *shader, const char *vertexSource,
                  const char *fragmentSource) {
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertexShader, 1, &vertexSource, NULL);
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

  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s\n", infoLog);
  }

  *shader = glCreateProgram();

  glAttachShader(*shader, vertexShader);
  glAttachShader(*shader, fragmentShader);
  glLinkProgram(*shader);

  glGetProgramiv(*shader, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(*shader, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n %s\n", infoLog);
  }

  // These are not needed after linking
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
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

  // Set up shaders
  unsigned int shaderProgram;
  setup_shader(&shaderProgram, vertexShaderSource, fragmentShaderSource);

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

    glUseProgram(shaderProgram);

    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

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
