#ifndef VOLUME
#define VOLUME

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "plane.h"

class Volume {
  std::vector<Plane> sides;

public:
  Volume();
  Volume(glm::vec3 p1, glm::vec3 p2);
  std::vector<GLfloat> Triangles();
  std::vector<glm::vec3> Normals();
};

#endif