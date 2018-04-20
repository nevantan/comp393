#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <cmath>
using namespace std;
using namespace glm;

#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

const int WIDTH = 1024;
const int HEIGHT = 768;

GLFWwindow* window;
GLuint VertexArrayID;
GLuint vertexbuffer;
GLuint elementbuffer;

GLuint program;

mat4 mvp;
GLuint MVP;
GLuint MaxHeight;

float dampen = 16;

void initScene() {
  // Setup shaders
  program = Shader("shaders/shader.vert", "shaders/shader.frag").Program();

  // Setup MVP (camera) matrix and other uniforms
  mat4 projection = perspective(radians(90.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 300.0f);
  mat4 view = lookAt(
    vec3(100, 90, 220), // Position
    vec3(100, 8, 100),  // Target
    vec3(0, 1, 0)  // Up
  );
  mat4 model = mat4(1.0f);
  mvp = projection * view * model;
  MVP = glGetUniformLocation(program, "MVP");
  MaxHeight = glGetUniformLocation(program, "MaxHeight");

  // Load heightmap
  int iwidth, iheight, comp;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* image_data = stbi_load(
    "heightmap.png",
    &iwidth,
    &iheight,
    &comp,
    STBI_rgb
  );

  // Populate vertex data, using pixel intensity for y-coord
  GLfloat g_vertex_buffer_data[iwidth*iheight*3];
  for(int i = 0; i < iwidth*iheight*3; i+=3) {
    int row = floor((i/3) / iwidth);

    g_vertex_buffer_data[i+0] = (i / 3) % iwidth; // x
    g_vertex_buffer_data[i+1] = image_data[i] / dampen; // y/height
    g_vertex_buffer_data[i+2] = row; // z
  }

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  // Setup indices
  unsigned int g_element_buffer_data[(iwidth-1)*(iheight-1)*2*3];
  for(int i = 0; i < (iwidth-1)*(iheight-1)*2*3; i+= 6) {
    int row = floor((i/6) / (iwidth - 1));
    int x = ((i/2) - (row * iwidth)) / 3;
    int z = row / 3;

    g_element_buffer_data[i+0] = (i/6) + row;
    g_element_buffer_data[i+1] = (i/6) + row + 1;
    g_element_buffer_data[i+2] = (i/6) + row + iwidth;

    g_element_buffer_data[i+3] = (i/6) + row + 1;
    g_element_buffer_data[i+4] = (i/6) + row + iwidth;
    g_element_buffer_data[i+5] = (i/6) + row + iwidth + 1;
  }

  glGenBuffers(1, &elementbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_element_buffer_data), g_element_buffer_data, GL_STATIC_DRAW);
}

void drawScene() {
  // Setup shaders and uniforms
  glUseProgram(program);
  glUniformMatrix4fv(MVP, 1, GL_FALSE, &mvp[0][0]);
  glUniform1f(MaxHeight, 256 / dampen);

  // Draw terrain
  glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawElements(GL_TRIANGLES, 199*199*2*3, GL_UNSIGNED_INT, (void*)0);
  glDisableVertexAttribArray(0);
}

void render() {
  do {
    drawScene();

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while(
    glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0
  );
}

int main(int argc, char **argv) {
  // Create window
  if(!glfwInit()) {
    cout << "Failed to initialize GLFW" << endl;
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WIDTH, HEIGHT, "COMP393 - Assignment 4", NULL, NULL);
  if(window == NULL) {
    cout << "Failed to open GLFW window" << endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glewExperimental = true;
  if(glewInit() != GLEW_OK) {
    cout << "Failed to initialize GLFW" << endl;
    return -1;
  }

  // Setup VAO
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Set to wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Initialize the scene
  initScene();

  // Setup for input
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Start render loop
  render();

  return 0;
}