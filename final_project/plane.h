#ifndef PLANE
#define PLANE

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Plane {
  GLfloat x, y, w, h;
  glm::vec3 normal;
  std::vector<GLfloat> vertices;

public:
  Plane();
  Plane(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, glm::vec3 normal);
  std::vector<GLfloat> Vertices();
  std::vector<GLfloat> Triangles();
};

#endif