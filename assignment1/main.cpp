// Athabasca University
// Unit 5 Assignment 1

#include "shader.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace std;
using namespace glm;

int width, height;
GLFWwindow* window;
GLuint programID;

GLuint vertexbuffer;
GLuint colorbuffer;

mat4 mvp;
GLuint MatrixID;

mat4 initCamera() {
  mat4 Projection = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
  mat4 View = lookAt(
    vec3(-5.0f, 0.0f, 21.0f), // Pos
    vec3(0.0f, 0.0f, 0.0f), // Look at
    vec3(0.0f, 1.0f, 0.0f)  // Up vector
  );
  mat4 Model = mat4(1.0f);
  return Projection * View * Model;
}

void initScene() {
  static const GLfloat g_vertex_buffer_data[] = {
    -8.0, -6.0, 0.0, // Wall 1
    -8.0, 3.0, 0.0,
    8.0, 3.0, 0.0,
    -8.0, -6.0, 0.0,
    8.0, 3.0, 0.0,
    8.0, -6.0, 0.0,
    8.0, -6.0, 0.0, // Wall 2
    8.0, 3.0, 0.0,
    8.0, 3.0, 6.0,
    8.0, -6.0, 0.0,
    8.0, 3.0, 6.0,
    8.0, -6.0, 6.0,
    -8.0, -6.0, 6.0, // Floor
    -8.0, -6.0, 0.0,
    8.0, -6.0, 0.0,
    -8.0, -6.0, 6.0,
    8.0, -6.0, 0.0,
    8.0, -6.0, 6.0,
    -5.5, -0.854, 0.05, // Frame 1
    -5.5, 1.0, 0.05,
    -2.5, 1.0, 0.05,
    -5.5, -0.854, 0.05,
    -2.5, 1.0, 0.05,
    -2.5, -0.854, 0.05,
    1.5, -0.854, 0.05, // Frame 2
    1.5, 1.0, 0.05,
    4.5, 1.0, 0.05,
    1.5, -0.854, 0.05,
    4.5, 1.0, 0.05,
    4.5, -0.854, 0.05,
    7.95, -2.0, 1.5, // Frame 3
    7.95, 1.0, 1.5,
    7.95, 1.0, 2.646,
    7.95, -2.0, 1.5,
    7.95, 1.0, 2.646,
    7.95, -2.0, 2.646
  };

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  static const GLfloat g_color_buffer_data[] = {
    0.3, 0.3, 0.3, // Wall 1
    0.3, 0.3, 0.3,
    0.3, 0.3, 0.3,
    0.3, 0.3, 0.3,
    0.3, 0.3, 0.3,
    0.3, 0.3, 0.3,
    0.4, 0.4, 0.4, // Wall 2
    0.4, 0.4, 0.4,
    0.4, 0.4, 0.4,
    0.4, 0.4, 0.4,
    0.4, 0.4, 0.4,
    0.4, 0.4, 0.4,
    0.5, 0.5, 0.2, // Floor
    0.5, 0.5, 0.2,
    0.5, 0.5, 0.2,
    0.5, 0.5, 0.2,
    0.5, 0.5, 0.2,
    0.5, 0.5, 0.2,
    0.2, 0.6, 0.2, // Frame 1
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2, // Frame 2
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2, // Frame 3
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2,
    0.2, 0.6, 0.2
  };

  glGenBuffers(1, &colorbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
}

void initialize(int argc, char * argv[]) {
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  programID = Shader("shaders/vertex.glsl", "shaders/fragment.glsl").Program();

  mvp = initCamera();
  MatrixID = glGetUniformLocation(programID, "MVP");

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  initScene();

  // set background color
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  // enable depth test
  glEnable(GL_DEPTH_TEST);
}

void drawScene() {
  glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 12*3);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void render() {
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

    drawScene();

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

int main(int argc, char * argv[]) {
  // Init glfw
  if(!glfwInit()) {
    cerr << "Failed to initialize GLFW\n" << endl;
    return -1;
  }

  // Init window
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  width = 500;
  height = 500;
  window = glfwCreateWindow(width, height, "COMP393 Assignment 1", NULL, NULL);
  if(window == NULL) {
    cerr << "Failed to open GLFW window." << endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Init glew
  glewExperimental = true;
  if(glewInit() != GLEW_OK) {
    cerr << "Failed to initialize GLEW" << endl;
    return -1;
  }

  // Init program
  initialize(argc, argv);

  // Start render loop
  render();

  // Exit
  return 0;
}