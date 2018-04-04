#include "GLShader.h"
#include <iostream>
using namespace std;
using namespace glm;

int main(int argc, char **argv) {
  // Initialize GLFW
  if(!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }

  // Window Settings
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create window and check that it is open
  GLFWwindow* window;
  int width = 1024;
  int height = 768;
  window = glfwCreateWindow(width, height, "Tutorial 01", NULL, NULL);
  if(window == NULL) {
    fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Init GLEW
  glewExperimental = true;
  if(glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
  }

  // Setup Vertex Array Object
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Load Shaders
  GLuint programID = LoadShader(argv[1], argv[2]);

  // Setup MVP matrix
  mat4 Projection = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
  mat4 View = lookAt(
    vec3(4, 3, 3), // Camera a (4, 3, 3) world
    vec3(0, 0, 0), // Looking at origin
    vec3(0, 1, 0)  // Head is up
  );
  mat4 Model = mat4(1.0f); // Identity matrix because model is at origin
  mat4 mvp = Projection * View * Model;

  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  // Draw Triangle
  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  // Setup input
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Loop to keep window open and poll input
  do {
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 12*3);
    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

  return 0;
}