#include <shaders.h>

#include <glad.h>

#include <stdio.h>
#include <stdlib.h>

char *file_read(const char *filename) {
  FILE *file = fopen(filename, "rb");

  if (file == NULL) {
    printf("[file_read] Unable to read file: %s\n", filename);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = (char *)malloc(length + 1);
  if (buffer == NULL) {
    fclose(file);
    return NULL;
  }

  fread(buffer, 1, length, file);
  buffer[length] = '\0';

  fclose(file);

  return buffer;
}

unsigned int shader_create(const char *vertex_path, const char *fragment_path) {
  const char *vertex_source = file_read(vertex_path);
  const char *fragment_source = file_read(fragment_path);

  int success;
  char infoLog[512];

  unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertex_source, NULL);
  glCompileShader(vertex);

  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    printf("[shader_create][vertex][compile]: %s\n", infoLog);
  };

  unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragment_source, NULL);
  glCompileShader(fragment);

  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    printf("[shader_create][fragment][compile]: %s\n", infoLog);
  };

  unsigned int id = glCreateProgram();
  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  glLinkProgram(id);

  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 512, NULL, infoLog);
    printf("[shader_create][program][link]: %s\n", infoLog);
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  free((void *)vertex_source);
  free((void *)fragment_source);

  return id;
}

void shader_use(unsigned int id) { glUseProgram(id); }

void shader_set_bool(unsigned int id, const char *name, bool value) {
  glUniform1i(glGetUniformLocation(id, name), (int)value);
}

void shader_set_int(unsigned int id, const char *name, int value) {
  glUniform1i(glGetUniformLocation(id, name), value);
}

void shader_set_float(unsigned int id, const char *name, float value) {
  glUniform1f(glGetUniformLocation(id, name), value);
}

void shader_set_vec3(unsigned int id, const char *name, float x, float y,
                     float z) {
  glUniform3f(glGetUniformLocation(id, name), x, y, z);
}

void shader_set_vec3v(unsigned int id, const char *name, vec3 vec) {
  glUniform3fv(glGetUniformLocation(id, name), 1, (float *)vec);
}

void shader_set_mat4(unsigned int id, const char *name, mat4 mat) {
  glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE,
                     (float *)mat[0]);
}
