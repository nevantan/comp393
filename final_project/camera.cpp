#include "camera.h"
using namespace glm;

void Camera::init(float fov, float aspect, float near, float far, vec3 pos, vec3 target, vec3 up) {
  projection = perspective(radians(fov), aspect, near, far);
  model = mat4(1.0f);
  recalcView(pos, target, up);
}

void Camera::recalcView(vec3 pos, vec3 target, vec3 up) {
  this->pos = pos;
  this->target = target;
  this->up = up;

  view = lookAt(pos, target, up);
  mvp = projection * view * model;
}

Camera::Camera() {}

Camera::Camera(float aspect, vec3 pos, vec3 target, vec3 up) {
  init(45.0f, aspect, 0.1f, 100.0f, pos, target, up);
}

Camera::Camera(float fov, float aspect, float near, float far, vec3 pos, vec3 target, vec3 up) {
  init(fov, aspect, near, far, pos, target, up);
}

mat4 Camera::Model() {
  return model;
}

mat4 Camera::View() {
  return view;
}

mat4 Camera::Projection() {
  return projection;
}

mat4 Camera::MVP() {
  return mvp;
}

void Camera::MoveTo(vec3 pos) {
  recalcView(pos, target, up);
}

void Camera::LookAt(vec3 target) {
  recalcView(pos, target, up);
}

vec3 Camera::Position() {
  return pos;
}