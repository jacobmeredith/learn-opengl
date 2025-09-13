#ifndef SHADERS_H
#define SHADERS_H

#include "cglm/types.h"
#include <stdbool.h>

// TODO: move this to an io file
char *file_read(const char *filename);

unsigned int shader_create(const char *vertexPath, const char *fragmentPath);
void shader_use(unsigned int id);

void shader_set_bool(unsigned int id, const char *name, bool value);
void shader_set_int(unsigned int id, const char *name, int value);
void shader_set_float(unsigned int id, const char *name, float value);

void shader_set_vec3(unsigned int id, const char *name, float x, float y,
                     float z);
void shader_set_vec3v(unsigned int id, const char *name, vec3 vec);

void shader_set_mat4(unsigned int id, const char *name, mat4 mat);

#endif
