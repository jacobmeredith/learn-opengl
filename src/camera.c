#include <camera.h>
#include <cglm/cam.h>
#include <cglm/types.h>
#include <cglm/vec3.h>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

void camera_init(Camera *camera, vec3 position, vec3 up, float yaw,
                 float pitch) {
  glm_vec3_copy(position, camera->Position);
  glm_vec3_copy(up, camera->WorldUp);
  camera->Yaw = yaw;
  camera->Pitch = pitch;
  camera->MovementSpeed = SPEED;
  camera->MouseSensitivity = SENSITIVITY;
  camera->Zoom = ZOOM;
  camera_update_camera_vectors(camera);
}

void camera_get_view_matrix(Camera *camera, mat4 dest) {
  vec3 center;
  glm_vec3_add(camera->Position, camera->Front, center);
  glm_lookat(camera->Position, center, camera->Up, dest);
}

void camera_process_keyboard(Camera *camera, Camera_Movement direction,
                             float delta_time) {
  float velocity = camera->MovementSpeed * delta_time;
  if (direction == FORWARD)
    glm_vec3_muladds(camera->Front, velocity, camera->Position);
  if (direction == BACKWARD)
    glm_vec3_muladds(camera->Front, -velocity, camera->Position);
  if (direction == LEFT)
    glm_vec3_muladds(camera->Right, -velocity, camera->Position);
  if (direction == RIGHT)
    glm_vec3_muladds(camera->Right, velocity, camera->Position);
}

void camera_process_mouse_movement(Camera *camera, float xoffset, float yoffset,
                                   bool constrain_pitch) {
  xoffset *= camera->MouseSensitivity;
  yoffset *= camera->MouseSensitivity;

  camera->Yaw += xoffset;
  camera->Pitch += yoffset;

  if (constrain_pitch) {
    if (camera->Pitch > 89.0f)
      camera->Pitch = 89.0f;
    if (camera->Pitch < -89.0f)
      camera->Pitch = -89.0f;
  }
  camera_update_camera_vectors(camera);
}

void camera_process_mouse_scroll(Camera *camera, float yoffset) {
  camera->Zoom -= (float)yoffset;
  if (camera->Zoom < 1.0f)
    camera->Zoom = 1.0f;
  if (camera->Zoom > 45.0f)
    camera->Zoom = 45.0f;
}

void camera_update_camera_vectors(Camera *camera) {
  vec3 front;
  front[0] = cos(glm_rad(camera->Yaw)) * cos(glm_rad(camera->Pitch));
  front[1] = sin(glm_rad(camera->Pitch));
  front[2] = sin(glm_rad(camera->Yaw)) * cos(glm_rad(camera->Pitch));
  glm_vec3_normalize_to(front, camera->Front);

  glm_vec3_cross(camera->Front, camera->WorldUp, camera->Right);
  glm_vec3_normalize(camera->Right);

  glm_vec3_cross(camera->Right, camera->Front, camera->Up);
  glm_vec3_normalize(camera->Up);
}
