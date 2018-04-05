// Athabasca University
// Unit 5 Assignment 1

#include "shader.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace std;
using namespace glm;

GLFWwindow* window;
GLuint programID;
GLuint vertexbuffer;

void initScene() {
  static const GLfloat g_vertex_buffer_data[] = {
    -8.0, -6.0, 0.0, // Wall 1
    -8.0, 3.0, 0.0,
    8.0, 3.0, 0.0,
    8.0, 3.0, 0.0,
    8.0, -6.0, 0.0,
    -8.0, -6.0, 0.0
  };

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

void initialize(int argc, char * argv[]) {
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  programID = Shader("shaders/vertex.glsl", "shaders/fragment.glsl").Program();

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  initScene();

  // set background color
  glClearColor(0.3, 0.3, 0.7, 0.0);

  // enable depth test
  glEnable(GL_DEPTH_TEST);
}

void drawScene() {
  glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 6, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(0);
}

void drawPicture() {
  glColor3f(1.0, 1.0, 1.0);

  // wall 1
  glColor3f(0.3, 0.3, 0.3);
  glBegin( GL_POLYGON );
    glVertex3f(-8.0, -6.0, 0.0);
    glVertex3f(-8.0, 3.0, 0.0);
    glVertex3f(8.0, 3.0, 0.0);
    glVertex3f(8.0, -6.0, 0.0);
  glEnd();

  // wall 2
  glColor3f(0.4, 0.4, 0.4);
  glBegin( GL_POLYGON );
    glVertex3f(8.0, -6.0, 0.0);
    glVertex3f(8.0, 3.0, 0.0);
    glVertex3f(8.0, 3.0, 6.0);
    glVertex3f(8.0, -6.0, 6.0);
  glEnd();

  // floor
  glColor3f(0.5, 0.5, 0.2);
  glBegin( GL_POLYGON );
    glVertex3i(-8.0, -6.0, 6.0);
    glVertex3i(-8.0, -6.0, 0.0);
    glVertex3i(10.0, -6.0, 0.0);
    glVertex3i(10.0, -6.0, 6.0);
  glEnd();

  // frame 1
  glColor3f(0.2, 0.6, 0.2);
  glBegin( GL_POLYGON );
    glVertex3f(-5.5, -0.854, 0.05);
    glVertex3f(-5.5, 1.0, 0.05);
    glVertex3f(-2.5, 1.0, 0.05);
    glVertex3f(-2.5, -0.854, 0.05);
  glEnd();

  // frame 2
  glBegin( GL_POLYGON );
    glVertex3f(1.5, -0.854, 0.05);
    glVertex3f(1.5, 1.0, 0.05);
    glVertex3f(4.5, 1.0, 0.05);
    glVertex3f(4.5, -0.854, 0.05);
  glEnd();

  // frame 3
  glBegin( GL_POLYGON );
    glVertex3f(7.95, -2.0, 1.5);
    glVertex3f(7.95, 1.0, 1.5);
    glVertex3f(7.95, 1.0, 2.646);
    glVertex3f(7.95, -2.0, 2.646);
  glEnd();
}

void render() {
  do {
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    drawScene();

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 40.0);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char * argv[])
{
  if(!glfwInit()) {
    cerr << "Failed to initialize GLFW\n" << endl;
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(500, 500, "COMP393 Assignment 1", NULL, NULL);
  if(window == NULL) {
    cerr << "Failed to open GLFW window." << endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  glewExperimental = true;
  if(glewInit() != GLEW_OK) {
    cerr << "Failed to initialize GLEW" << endl;
    return -1;
  }

  initialize(argc, argv);

  render();

  return 0;
}