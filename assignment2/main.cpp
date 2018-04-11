#include "shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <cmath>
using namespace std;
using namespace glm;

// Window variables
int width, height;
GLFWwindow* window;

// Shader variables
mat4 mvp;
GLuint MVP;
GLuint T;
GLuint program;
GLuint noiseProgram;
GLuint vertexbuffer;
float tick;

// Noise variables
int size = 256;
int seed = 57;
float randomTable[256][256];

struct array {
  float a[256][256][4];
};

// Initialization
void initRandom() {
  srand(seed);
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      randomTable[i][j] = (rand() % 1000) / 1000.0;
    }
  }
}

void initCamera() {
  mat4 Projection = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
  mat4 View = lookAt(
    vec3(-2.0f, 3.0f, 3.0f), // Position
    vec3(0.0f, 0.0f, 0.0f), // Look at
    vec3(0.0f, 1.0f, 0.0f) // Up vector
  );
  mat4 Model = mat4(1.0f);
  mvp = Projection * View * Model;
}

void initShaders() {
  program = Shader("shaders/vertex.glsl", "shaders/fragment.glsl").Program();
  noiseProgram = Shader("shaders/noise/noise.vert", "shaders/noise/noise.frag").Program();
  MVP = glGetUniformLocation(program, "MVP");
  T = glGetUniformLocation(program, "T");
}

void initScene() {
  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
  };

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  glClearColor(0.39f, 0.584, 0.929f, 0.0f);
  glEnable(GL_DEPTH_TEST);
}

// Common Functions
float interpolate(float a, float b, float x) {
  float f = (1 - cos(x * 3.1415926)) * 0.5;
  return (a * (1-f)) + (b * f);
}

float noise(int x, int y) {
  //x = std::max(0, std::min(x, size));
  //y = std::max(0, std::min(y, 256));
  return randomTable[x][y];
}

float smoothNoise(int x, int y) {
  float corners = (noise(x-1, y-1) + noise(x+1, y-1) + noise(x-1, y+1) + noise(x+1, y+1)) / 16;
  float sides = (noise(x-1, y) + noise(x+1, y) + noise(x, y-1) + noise(x, y+1) / 8);
  float center = noise(x, y) / 4;

  return corners + sides + center;
}

// Interpolate Noise
float interpolateNoise(float x, float y) {
  int intX = int(x);
  float fractX = x - intX;
  int intY = int(y);
  float fractY = y - intY;

  float v1 = smoothNoise(intX, intY);
  float v2 = smoothNoise(intX + 1, intY);
  float v3 = smoothNoise(intX, intY + 1);
  float v4 = smoothNoise(intX + 1, intY + 1);

  float i1 = interpolate(v1, v2, fractX);
  float i2 = interpolate(v3, v4, fractX);

  return interpolate(i1, i2, fractY);
}

// Create Noise
void makeNoise() {
  int frequency = 4;
  double amp = 0.3;
  int step = size / frequency;

  for(int i = 0; i < size; i++) {
    float locX = ((float)i) / step;
    for(int j = 0; j < size*4; j+=4) {
      float locY = ((float)j) / step;
      float n = interpolateNoise(locX, locY);
      
    }
  }
}

void drawNoise() {
  glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glUseProgram(noiseProgram);
      glUniformMatrix4fv(MVP, 1, GL_FALSE, &mvp[0][0]);
      glUniform1f(T, tick);
      glDrawArrays(GL_TRIANGLES, 0, 12*3);

  glDisableVertexAttribArray(0);
}

void render() {
  do {
    tick *= 1.3;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawNoise();

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

void init() {
  initRandom();
  initCamera();
  initShaders();
  initScene();

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
}

int main(int argc, const char** argv) {
  if(!glfwInit()) {
    cerr << "Failed to initialize GLFW" << endl;
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
  window = glfwCreateWindow(width, height, "COMP393 Assignment 2", NULL, NULL);
  if(window == NULL) {
    cerr << "Failed to open GLFW window." << endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Init glew
  glewExperimental = true;
  if(glewInit() != GLEW_OK) {
    cerr << "Failed to initialize GLEW" << endl;
    return -1;
  }

  init();

  tick = 3.0;
  render();

  return 0;
}