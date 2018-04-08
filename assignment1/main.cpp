// Athabasca University
// Unit 5 Assignment 1

#include "shader.h"
#include "texture.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

using namespace std;
using namespace glm;

int width, height;
GLFWwindow* window;
GLuint brickProgram;
GLuint woodProgram;
GLuint pictureProgram;

GLuint vertexbuffer;
GLuint uvbuffer;

mat4 mvp;
GLuint brickMVP;
GLuint brickColor;
GLuint mortarColor;
GLuint brickSize;
GLuint brickPct;
GLuint brickPlane;

GLuint woodMVP;
GLuint woodColor;
GLuint spacerColor;
GLuint woodSize;
GLuint woodPct;
GLuint woodPlane;

GLuint pictureMVP;
GLuint pictureTexture;
GLuint starryNight;

mat4 initCamera() {
  mat4 Projection = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
  mat4 View = lookAt(
    vec3(-5.0f, 5.0f, 21.0f), // Pos
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
    /*-5.5, -0.854, 0.05, // Frame 1
    -5.5, 1.0, 0.05,
    -2.5, 1.0, 0.05,
    -5.5, -0.854, 0.05,
    -2.5, 1.0, 0.05,
    -2.5, -0.854, 0.05,*/
    -5.5, -2.0, 0.05, // Temp Frame 1
    -5.5, 1.0, 0.05,
    -2.5, 1.0, 0.05,
    -5.5, -2.0, 0.05,
    -2.5, 1.0, 0.05,
    -2.5, -2.0, 0.05,
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

  static const GLfloat g_uv_buffer_data[] = {
    0.0, 0.0, // Wall 1
    0.0, 0.0,
    0.0, 0.0,
    0.0, 0.0,
    0.0, 0.0,
    0.0, 0.0,
    0.0, 0.0, // Wall 2
    0.0, 0.0,
    0.0, 0.0,
    0.0, 0.0,
    0.0, 0.0,
    0.0, 0.0,
    0.0, 0.0, // Floor
    0.0, 0.0,
    0.0, 0.0,
    0.0, 0.0,
    0.0, 0.0,
    0.0, 0.0,
    0.0, 0.0, // Frame 1
    0.0, 1.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 1.0,
    1.0, 0.0,
    0.0, 0.0, // Frame 2
    0.0, 1.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 1.0,
    1.0, 0.0,
    0.0, 0.0, // Frame 3
    0.0, 1.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 1.0,
    1.0, 0.0
  };

  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
}

void initialize(int argc, char * argv[]) {
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  brickProgram = Shader("shaders/brick/vertex.glsl", "shaders/brick/fragment.glsl").Program();
  woodProgram = Shader("shaders/wood/vertex.glsl", "shaders/wood/fragment.glsl").Program();
  pictureProgram = Shader("shaders/picture/vertex.glsl", "shaders/picture/fragment.glsl").Program();

  mvp = initCamera();

  brickMVP = glGetUniformLocation(brickProgram, "MVP");
  brickColor = glGetUniformLocation(brickProgram, "BrickColor");
  mortarColor = glGetUniformLocation(brickProgram, "MortarColor");
  brickSize = glGetUniformLocation(brickProgram, "BrickSize");
  brickPct = glGetUniformLocation(brickProgram , "BrickPct");
  brickPlane = glGetUniformLocation(brickProgram, "Plane");

  woodMVP = glGetUniformLocation(woodProgram, "MVP");
  woodColor = glGetUniformLocation(woodProgram, "WoodColor");
  spacerColor = glGetUniformLocation(woodProgram, "SpacerColor");
  woodSize = glGetUniformLocation(woodProgram, "WoodSize");
  woodPct = glGetUniformLocation(woodProgram, "WoodPct");
  woodPlane = glGetUniformLocation(woodProgram, "Plane");

  pictureMVP = glGetUniformLocation(pictureProgram, "MVP");

  Texture starryNightTex = Texture("images/starry_night.png");
  if(!starryNightTex.load()) {
    cout << "There was a problem loading Starry Night" << endl;
  }
  starryNight = starryNightTex.id();

  pictureTexture = glGetUniformLocation(pictureProgram, "Picture");

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
  // Bind Textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, starryNight);

  //glActiveTexture(GL_TEXTURE1);
  //glBindTexture(GL_TEXTURE_2D, blueHorsesTex);

  glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glUseProgram(brickProgram);
      glUniformMatrix4fv(brickMVP, 1, GL_FALSE, &mvp[0][0]);
      glUniform3f(brickColor, 1.0f, 0.3f, 0.3f);
      glUniform3f(mortarColor, 0.85f, 0.86f, 0.84f);
      glUniform2f(brickSize, 1.2f, 0.6f);
      glUniform2f(brickPct, 0.9f, 0.9f);
      glUniform3f(brickPlane, 1.0f, 1.0f, 0.0f);
      glDrawArrays(GL_TRIANGLES, 0, 6);

      glUniform3f(brickPlane, 0.0f, 1.0f, 1.0f);
      glDrawArrays(GL_TRIANGLES, 6, 6);

    glUseProgram(woodProgram);
      glUniformMatrix4fv(woodMVP, 1, GL_FALSE, &mvp[0][0]);
      glUniform3f(woodColor, 0.808f, 0.733f, 0.619f);
      glUniform3f(spacerColor, 0.686f, 0.565f, 0.381f);
      glUniform2f(woodSize, 2.0f, 0.4f);
      glUniform2f(woodPct, 0.98f, 0.88f);
      glUniform3f(woodPlane, 1.0f, 0.0f, 1.0f);
      glDrawArrays(GL_TRIANGLES, 12, 6);

    glUseProgram(pictureProgram);
      glUniformMatrix4fv(pictureMVP, 1, GL_FALSE, &mvp[0][0]);
      glUniform1i(pictureTexture, 0);
      glDrawArrays(GL_TRIANGLES, 18, 18);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void render() {
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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