#include "camera.h"
#include "shader.h"
#include "plane.h"
#include "volume.h"

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace std;
using namespace glm;

int width, height;
GLFWwindow* window;
GLuint VertexArrayID;

GLuint vertexbuffer;
GLuint colorbuffer;
GLuint normalbuffer;

Camera camera;
Shader basicShader;
Shader lightingShader;

void initScene() {
  camera = Camera(
    (float)width / (float)height, // Aspect
    vec3(-10, 12, 12), // Position
    vec3(0, 4, 0), // Target
    vec3(0, 1, 0)  // Up
  );

  // basicShader = Shader("shaders/basic.vert", "shaders/basic.frag");
  lightingShader = Shader("shaders/lighting.vert", "shaders/lighting.frag");

  // Room geometry
  Plane backWall = Plane(-4.0f, 6.0f, -4.0f, 8.0f, 6.0f, vec3(0, 0, 1));
  vector<GLfloat> backWallVertices = backWall.Triangles();
  vector<vec3> backWallNormals = backWall.Normals();

  Plane sideWall = Plane(4.0f, 6.0f, -4.0f, 8.0f, 6.0f, vec3(-1, 0, 0));
  vector<GLfloat> sideWallVertices = sideWall.Triangles();
  vector<vec3> sideWallNormals = sideWall.Normals();

  Plane floor = Plane(-4.0f, 0.0f, -4.0f, 8.0f, 8.0f, vec3(0, 1, 0));
  vector<GLfloat> floorVertices = floor.Triangles();
  vector<vec3> floorNormals = floor.Normals();

  Plane window1 = Plane(-1.0f, 4.0f, -3.95f, 2.0f, 2.0f, vec3(0, 0, 1));
  vector<GLfloat> window1Vertices = window1.Triangles();
  vector<vec3> window1Normals = window1.Normals();

  Plane window2 = Plane(3.95f, 4.0f, -1.0f, 2.0f, 2.0f, vec3(-1, 0, 0));
  vector<GLfloat> window2Vertices = window2.Triangles();
  vector<vec3> window2Normals = window2.Normals();

  // Table
  Volume tableSurface = Volume(vec3(-2.0, 2.2, -2.0), vec3(2.0, 2.0, 2.0));
  vector<GLfloat> tableSurfaceVertices = tableSurface.Triangles();
  vector<vec3> tableSurfaceNormals = tableSurface.Normals();

  Volume leg1 = Volume(vec3(-1.9, 2.15, -1.9), vec3(-1.8, 0.0, -1.8));
  vector<GLfloat> leg1Vertices = leg1.Triangles();
  vector<vec3> leg1Normals = leg1.Normals();

  Volume leg2 = Volume(vec3(1.9, 2.15, -1.9), vec3(1.8, 0.0, -1.8));
  vector<GLfloat> leg2Vertices = leg2.Triangles();
  vector<vec3> leg2Normals = leg2.Normals();

  Volume leg3 = Volume(vec3(-1.9, 2.15, 1.9), vec3(-1.8, 0.0, 1.8));
  vector<GLfloat> leg3Vertices = leg3.Triangles();
  vector<vec3> leg3Normals = leg3.Normals();

  Volume leg4 = Volume(vec3(1.9, 2.15, 1.9), vec3(1.8, 0.0, 1.8));
  vector<GLfloat> leg4Vertices = leg4.Triangles();
  vector<vec3> leg4Normals = leg4.Normals();

  // Candle
  Volume candle = Volume(vec3(1.2, 2.7, -1.2), vec3(1.3, 2.2, -1.1));
  vector<GLfloat> candleVertices = candle.Triangles();
  vector<vec3> candleNormals = candle.Normals();

  // Cube
  Volume cube = Volume(vec3(-1.2, 2.4, 1.2), vec3(-1.0, 2.2, 1.4));
  vector<GLfloat> cubeVertices = cube.Triangles();
  vector<vec3> cubeNormals = cube.Normals();

  // Build buffer data
  vector<GLfloat> g_vertex_buffer_data;

  // Room triangles
  g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), backWallVertices.begin(), backWallVertices.end());
  g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), sideWallVertices.begin(), sideWallVertices.end());
  g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), floorVertices.begin(), floorVertices.end());
  g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), window1Vertices.begin(), window1Vertices.end());
  g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), window2Vertices.begin(), window2Vertices.end());

  // Table triangles
  g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), tableSurfaceVertices.begin(), tableSurfaceVertices.end());
  g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), leg1Vertices.begin(), leg1Vertices.end());
  g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), leg2Vertices.begin(), leg2Vertices.end());
  g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), leg3Vertices.begin(), leg3Vertices.end());
  g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), leg4Vertices.begin(), leg4Vertices.end());

  // Objects
  g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), candleVertices.begin(), candleVertices.end());
  g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), cubeVertices.begin(), cubeVertices.end());

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(GLfloat), &g_vertex_buffer_data[0], GL_STATIC_DRAW);

  vector<GLfloat> g_color_buffer_data;
  for(int i = 0; i < 285*3; i++) {
    g_color_buffer_data.push_back(0.5f);
  }

  glGenBuffers(1, &colorbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, g_color_buffer_data.size() * sizeof(GLfloat), &g_color_buffer_data[0], GL_STATIC_DRAW);

  // Build normal buffer data
  vector<vec3> normals;

  // Room normals
  normals.insert(normals.end(), backWallNormals.begin(), backWallNormals.end());
  normals.insert(normals.end(), sideWallNormals.begin(), sideWallNormals.end());
  normals.insert(normals.end(), floorNormals.begin(), floorNormals.end());
  normals.insert(normals.end(), window1Normals.begin(), window1Normals.end());
  normals.insert(normals.end(), window2Normals.begin(), window2Normals.end());

  // Table normals
  normals.insert(normals.end(), tableSurfaceNormals.begin(), tableSurfaceNormals.end());
  normals.insert(normals.end(), leg1Normals.begin(), leg1Normals.end());
  normals.insert(normals.end(), leg2Normals.begin(), leg2Normals.end());
  normals.insert(normals.end(), leg3Normals.begin(), leg3Normals.end());
  normals.insert(normals.end(), leg4Normals.begin(), leg4Normals.end());

  // Object normals
  normals.insert(normals.end(), candleNormals.begin(), candleNormals.end());
  normals.insert(normals.end(), cubeNormals.begin(), cubeNormals.end());

  //cout << normals[0].x << ", " << normals[0].y << ", " << normals[0].z << endl;

  glGenBuffers(1, &normalbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
}

void drawScene() {
  glUseProgram(lightingShader.Program());
  glUniformMatrix4fv(lightingShader.Uniform("Model"), 1, GL_FALSE, &camera.Model()[0][0]);
  glUniformMatrix4fv(lightingShader.Uniform("View"), 1, GL_FALSE, &camera.View()[0][0]);
  glUniformMatrix4fv(lightingShader.Uniform("Projection"), 1, GL_FALSE, &camera.Projection()[0][0]);
  glUniform3f(lightingShader.Uniform("LightColor"), 1.0f, 1.0f, 1.0f);
  glUniform3f(lightingShader.Uniform("CameraPosition"), camera.Position().x, camera.Position().y, camera.Position().z);
  glUniform3f(lightingShader.Uniform("material.ambient"), 1.0f, 0.5f, 0.31f);
  glUniform3f(lightingShader.Uniform("material.diffuse"), 1.0f, 0.5f, 0.31f);
  glUniform3f(lightingShader.Uniform("material.specular"), 0.5f, 0.5f, 0.5f);
  glUniform1f(lightingShader.Uniform("material.shininess"), 32.0f);
  // glUniform3f(lightingShader.Uniform("light.position"), 1.25f, 2.8f, -1.15f);
  glUniform3f(lightingShader.Uniform("light.position"), 0.0f, 3.0f, -4.05f);
  glUniform3f(lightingShader.Uniform("light.ambient"), 0.2f, 0.2f, 0.2f);
  glUniform3f(lightingShader.Uniform("light.diffuse"), 0.5f, 0.5f, 0.5f);
  glUniform3f(lightingShader.Uniform("light.specular"), 1.0f, 1.0f, 1.0f);

  glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 285);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

void render() {
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawScene();

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while (
    glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0
  );
}

int main() {
  glewExperimental = true;
  if(!glfwInit()) {
    cerr << "Failed to initialize GLFW" << endl;
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  width = 1024;
  height = 768;
  window = glfwCreateWindow(width, height, "Programming Project", NULL, NULL);
  if(window == NULL) {
    cerr << "Failed to open GLFW window" << endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  if(glewInit() != GLEW_OK) {
    cerr << "Failed to initialize GLEW" << endl;
    return -1;
  }

  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  initScene();
  render();

  return 0;
}