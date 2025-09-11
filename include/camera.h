#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/types.h>
#include <stdbool.h>

extern const float YAW;
extern const float PITCH;
extern const float SPEED;
extern const float SENSITIVITY;
extern const float ZOOM;

typedef enum { FORWARD, BACKWARD, LEFT, RIGHT } Camera_Movement;

typedef struct {
  vec3 Position;
  vec3 Front;
  vec3 Up;
  vec3 Right;
  vec3 WorldUp;

  float Yaw;
  float Pitch;

  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;
} Camera;

void camera_init(Camera *camera, vec3 position, vec3 up, float yaw,
                 float pitch);
void camera_get_view_matrix(Camera *camera, mat4 dest);
void camera_process_keyboard(Camera *camera, Camera_Movement direction,
                             float delta_time);
void camera_process_mouse_movement(Camera *camera, float xoffset, float yoffset,
                                   bool constrain_pitch);
void camera_process_mouse_scroll(Camera *camera, float yoffset);
void camera_update_camera_vectors(Camera *camera);
#endif
