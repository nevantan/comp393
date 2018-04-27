#include "volume.h"
using namespace glm;
using namespace std;

Volume::Volume(vec3 p1, vec3 p2) {
  sides.push_back(Plane(p1, vec2(p2.x - p1.x, p2.z - p1.z), vec3(0, 1, 0))); // Top
  sides.push_back(Plane(p1, vec2(p2.x - p1.x, p1.y - p2.y), vec3(0, 0, 1))); // Back
  sides.push_back(Plane(vec3(p2.x, p1.y, p1.z), vec2(p2.z - p1.z, p1.y - p2.y), vec3(1, 0, 0))); // Right
  sides.push_back(Plane(vec3(p1.x, p2.y, p1.z), vec2(p2.x - p1.x, p2.z - p1.z), vec3(0, 1, 0))); // Bottom
  sides.push_back(Plane(p1, vec2(p2.z - p1.z, p1.y - p2.y), vec3(1, 0, 0))); // Left
  sides.push_back(Plane(vec3(p1.x, p1.y, p2.z), vec2(p2.x - p1.x, p1.y - p2.y), vec3(0, 0, 1))); // Front
}

vector<GLfloat> Volume::Triangles() {
  vector<GLfloat> vertex;

  for(int i = 0; i < sides.size(); i++) {
    vector<GLfloat> triangles = sides[i].Triangles();
    vertex.insert(vertex.end(), triangles.begin(), triangles.end());
  }

  return vertex;
}