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

Camera camera;
Shader basicShader;

void initScene() {
  camera = Camera(
    (float)width / (float)height, // Aspect
    vec3(-10, 12, 12), // Position
    vec3(0, 4, 0), // Target
    vec3(0, 1, 0)  // Up
  );

  basicShader = Shader("shaders/basic.vert", "shaders/basic.frag");

  static const GLfloat g_vertex_buffer_data_arr[] = {
    4.0f, 2.0f, -1.0f, // Window 2
    4.0f, 4.0f, -1.0f,
    4.0f, 4.0f, 1.0f,
    4.0f, 2.0f, -1.0f,
    4.0f, 4.0f, 1.0f,
    4.0f, 2.0f, 1.0f
  };
  Plane backWall = Plane(-4.0f, 6.0f, -4.0f, 8.0f, 6.0f, vec3(0, 0, 1));
  vector<GLfloat> backWallVertices = backWall.Triangles();

  Plane sideWall = Plane(4.0f, 6.0f, -4.0f, 8.0f, 6.0f, vec3(1, 0, 0));
  vector<GLfloat> sideWallVertices = sideWall.Triangles();

  Plane floor = Plane(-4.0f, 0.0f, -4.0f, 8.0f, 8.0f, vec3(0, 1, 0));
  vector<GLfloat> floorVertices = floor.Triangles();

  Plane window1 = Plane(-1.0f, 4.0f, -3.95f, 2.0f, 2.0f, vec3(0, 0, 1));
  vector<GLfloat> window1Vertices = window1.Triangles();

  Plane window2 = Plane(3.95f, 4.0f, -1.0f, 2.0f, 2.0f, vec3(1, 0, 0));
  vector<GLfloat> window2Vertices = window2.Triangles();

  // Table
  Volume tableSurface = Volume(vec3(-2.0, 2.2, -2.0), vec3(2.0, 2.0, 2.0));
  vector<GLfloat> tableSurfaceVertices = tableSurface.Triangles();

  Volume leg1 = Volume(vec3(-1.9, 2.15, -1.9), vec3(-1.8, 0.0, -1.8));
  vector<GLfloat> leg1Vertices = leg1.Triangles();

  Volume leg2 = Volume(vec3(1.9, 2.15, -1.9), vec3(1.8, 0.0, -1.8));
  vector<GLfloat> leg2Vertices = leg2.Triangles();

  Volume leg3 = Volume(vec3(-1.9, 2.15, 1.9), vec3(-1.8, 0.0, 1.8));
  vector<GLfloat> leg3Vertices = leg3.Triangles();

  Volume leg4 = Volume(vec3(1.9, 2.15, 1.9), vec3(1.8, 0.0, 1.8));
  vector<GLfloat> leg4Vertices = leg4.Triangles();

  vector<GLfloat> g_vertex_buffer_data;
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

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(GLfloat), &g_vertex_buffer_data[0], GL_STATIC_DRAW);

  static const GLfloat g_color_buffer_data[] = {
    0.5f, 0.5f, 0.5f, // Back wall
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.6f, 0.6f, 0.6f, // Side wall
    0.6f, 0.6f, 0.6f,
    0.6f, 0.6f, 0.6f,
    0.6f, 0.6f, 0.6f,
    0.6f, 0.6f, 0.6f,
    0.6f, 0.6f, 0.6f,
    0.4f, 0.4f, 0.4f, // Floor
    0.4f, 0.4f, 0.4f,
    0.4f, 0.4f, 0.4f,
    0.4f, 0.4f, 0.4f,
    0.4f, 0.4f, 0.4f,
    0.4f, 0.4f, 0.4f,
    0.9f, 0.9f, 0.9f, // Window 1
    0.9f, 0.9f, 0.9f,
    0.9f, 0.9f, 0.9f,
    0.9f, 0.9f, 0.9f,
    0.9f, 0.9f, 0.9f,
    0.9f, 0.9f, 0.9f,
    0.9f, 0.9f, 0.9f, // Window 2
    0.9f, 0.9f, 0.9f,
    0.9f, 0.9f, 0.9f,
    0.9f, 0.9f, 0.9f,
    0.9f, 0.9f, 0.9f,
    0.9f, 0.9f, 0.9f,
    1.0f, 0.0f, 0.0f, // Volume - Top
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f, // Volume - Back
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f, // Volume - Right
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.9f, 0.0f, 0.0f, // Volume - Bottom
    0.9f, 0.0f, 0.0f,
    0.9f, 0.0f, 0.0f,
    0.9f, 0.0f, 0.0f,
    0.9f, 0.0f, 0.0f,
    0.9f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f, // Volume - Left
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f, // Volume - Front
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
  };

  glGenBuffers(1, &colorbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
}

void drawScene() {
  glUseProgram(basicShader.Program());
  glUniformMatrix4fv(basicShader.Uniform("MVP"), 1, GL_FALSE, &camera.MVP()[0][0]);

  glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 213);
  glDisableVertexAttribArray(0);
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