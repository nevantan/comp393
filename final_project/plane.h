#ifndef PLANE
#define PLANE

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Plane {
  GLfloat x, y, w, h;
  glm::vec3 normal;
  std::vector<GLfloat> vertices;

  void init(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, glm::vec3 normal);

public:
  Plane();
  Plane(glm::vec3 p, glm::vec2 s, glm::vec3 normal);
  Plane(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, glm::vec3 normal);
  std::vector<GLfloat> Vertices();
  std::vector<GLfloat> Triangles();
  std::vector<glm::vec3> Normals();
};

#endif