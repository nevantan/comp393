#include "plane.h"
#include <iostream>
#include <stdlib.h>
using namespace std;
using namespace glm;

void Plane::init(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, vec3 normal) {
  this->normal = normal;

  float nx = abs(normal.x);

  vertices.push_back(x); vertices.push_back(y); vertices.push_back(z); // Upper-left
  vertices.push_back(x + (w * (1-nx))); vertices.push_back(y); vertices.push_back(z + (w * nx)); // Upper-right
  vertices.push_back(x); vertices.push_back(y - (h * (1-normal.y))); vertices.push_back(z + (h * normal.y)); // Lower-left
  vertices.push_back(x + (w * (1-nx))); vertices.push_back(y - (h * (1-normal.y))); vertices.push_back(z + (h * normal.y) + (w * nx)); // Lower-right
}

Plane::Plane() {}

Plane::Plane(vec3 p, vec2 s, vec3 normal) {
  init(p.x, p.y, p.z, s.x, s.y, normal);
}

Plane::Plane(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, vec3 normal) {
  init(x, y, z, w, h, normal);
}

vector<GLfloat> Plane::Vertices() {
  return vertices;
}

vector<GLfloat> Plane::Triangles() {
  vector<GLfloat> vertex;
  vertex.push_back(vertices[0]); vertex.push_back(vertices[1]); vertex.push_back(vertices[2]); // 0
  vertex.push_back(vertices[3]); vertex.push_back(vertices[4]); vertex.push_back(vertices[5]); // 1
  vertex.push_back(vertices[6]); vertex.push_back(vertices[7]); vertex.push_back(vertices[8]); // 2
  vertex.push_back(vertices[6]); vertex.push_back(vertices[7]); vertex.push_back(vertices[8]); // 2
  vertex.push_back(vertices[3]); vertex.push_back(vertices[4]); vertex.push_back(vertices[5]); // 1
  vertex.push_back(vertices[9]); vertex.push_back(vertices[10]); vertex.push_back(vertices[11]); // 3
  return vertex;
}

vector<vec3> Plane::Normals() {
  vector<vec3> normals;
  const int NUM_VERTICES = 6;
  for(int i = 0; i < NUM_VERTICES; i++) {
    normals.push_back(normal);
  }
  return normals;
}

vector<GLfloat> Plane::UVs() {
  vector<GLfloat> uvs;
  uvs.push_back(0.0f); uvs.push_back(1.0f); // 0
  uvs.push_back(1.0f); uvs.push_back(1.0f); // 1
  uvs.push_back(0.0f); uvs.push_back(0.0f); // 2
  uvs.push_back(0.0f); uvs.push_back(0.0f); // 2
  uvs.push_back(1.0f); uvs.push_back(1.0f); // 1
  uvs.push_back(1.0f); uvs.push_back(0.0f); // 3
  return uvs;
}