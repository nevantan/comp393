#include "shader.h"
#include "texture.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace std;
using namespace glm;

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

int width, height;
GLFWwindow* window;
GLuint VertexArrayID;

GLuint vertexbuffer;
GLuint elementbuffer;
GLuint uvbuffer;
Shader textureShader;
Shader reflectionShader;
mat4 mvp;

GLuint skyline;
GLuint normalMap;

void initMVP() {
  mat4 Projection = perspective(
    radians(45.0f), // FOV
    (float)width / (float)height, // Aspect ratio
    0.1f, // Near clipping plane
    100.0f // Far clipping plane
  );

  mat4 View = lookAt(
    vec3(0, 4, 6), // Position
    vec3(0, 1, 0), // Target
    vec3(0, 1, 0)  // Up Vector
  );

  mat4 Model = mat4(1.0f);

  mvp = Projection * View * Model;
}

void initTextures() {
  Texture skylineTex = Texture("images/skyline.png");
  if(!skylineTex.load()) {
    cerr << "There was a problem loading the skyline texture" << endl;
    return;
  }
  skyline = skylineTex.id();

  Texture normalMapTex = Texture("images/normal.png");
  if(!normalMapTex.load()) {
    cerr << "There was a problem loading the normal map" << endl;
    return;
  }
  normalMap = normalMapTex.id();
}

void initScene() {
  textureShader = Shader("shaders/texture.vert", "shaders/texture.frag");
  reflectionShader = Shader("shaders/reflection.vert", "shaders/reflection.frag");

  initMVP();
  initTextures();

  static const GLfloat g_vertex_buffer_data[] = {
     -4.0, 0.0, 0.0,
     -4.0, 4.0, 0.0,
     4.0, 4.0, 0.0,
     -4.0, 0.0, 0.0,
     4.0, 4.0, 0.0,
     4.0, 0.0, 0.0,
     -4.0, 0.0, 4.0,
     -4.0, 0.0, 0.0,
     4.0, 0.0, 0.0,
     -4.0, 0.0, 4.0,
     4.0, 0.0, 0.0,
     4.0, 0.0, 4.0
  };

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  static const GLfloat g_uv_buffer_data[] = {
    0.0, 0.0,
    0.0, 1.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 1.0,
    1.0, 0.0,
    0.0, 1.0,
    0.0, 0.0,
    1.0, 0.0,
    0.0, 1.0,
    1.0, 0.0,
    1.0, 1.0
  };

  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
}

void drawScene() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, skyline);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, normalMap);

  glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glUseProgram(textureShader.Program());
    glUniformMatrix4fv(textureShader.Uniform("MVP"), 1, GL_FALSE, &mvp[0][0]);
    glUniform1i(textureShader.Uniform("Picture"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(reflectionShader.Program());
    glUniformMatrix4fv(reflectionShader.Uniform("MVP"), 1, GL_FALSE, &mvp[0][0]);
    glUniform1i(reflectionShader.Uniform("NormalMap"), 1);
    glDrawArrays(GL_TRIANGLES, 6, 12);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void render() {
  do {
    glClear(GL_COLOR_BUFFER_BIT);

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
  window = glfwCreateWindow(width, height, "Assignment 5 - UV Map", NULL, NULL);
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

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  initScene();
  render();

  return 0;
}