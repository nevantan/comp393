#ifndef CAMERA
#define CAMERA

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera {
  glm::vec3 pos;
  glm::vec3 target;
  glm::vec3 up;

  glm::mat4 projection;
  glm::mat4 view;
  glm::mat4 model;
  glm::mat4 mvp;

  void init(float fov, float aspect, float near, float far, glm::vec3 pos, glm::vec3 target, glm::vec3 up);
  void recalcView(glm::vec3 pos, glm::vec3 target, glm::vec3 up);

public:
  Camera();
  Camera(float aspect, glm::vec3 pos, glm::vec3 target, glm::vec3 up);
  Camera(float fov, float aspect, float near, float far, glm::vec3 pos, glm::vec3 target, glm::vec3 up);

  glm::mat4 MVP();
  glm::mat4 Model();
  glm::mat4 View();
  glm::mat4 Projection();

  void MoveTo(glm::vec3 pos);
  void LookAt(glm::vec3 target);
};

#endif