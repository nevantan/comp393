#include "camera.h"
#include "shader.h"
#include "plane.h"
#include "volume.h"
#include "texture.h"

#include <iostream>
#include <vector>
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

GLuint depthMapFBO;
const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
GLuint depthMap;

GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;

Camera camera;
Shader lightingShader;
Shader brickShader;
Shader depthShader;
Shader shadowTestShader;

Texture brickDiffuse = Texture("textures/Brickwork_001_Diffuse.png");
Texture brickSpecular = Texture("textures/Brickwork_001_Specular.png");
Texture hardwoodDiffuse = Texture("textures/Hardwood_Diffuse.png");
Texture hardwoodSpecular = Texture("textures/Hardwood_Specular.png");

void initTextures() {
  if(!brickDiffuse.load()) {
    cerr << "There was a problem loading the brickwork diffuse texture" << endl;
    return;
  }

  if(!brickSpecular.load()) {
    cerr << "There was a problem loading the brickwork specular texture" << endl;
    return;
  }

  if(!hardwoodDiffuse.load()) {
    cerr << "There was a problem loading the hardwood diffuse texture" << endl;
    return;
  }

  if(!hardwoodSpecular.load()) {
    cerr << "There was a problem loading the hardwood specular texture" << endl;
    return;
  }
}

void initShadows() {
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void initScene() {
  camera = Camera(
    (float)width / (float)height, // Aspect
    vec3(-10, 12, 12), // Position
    vec3(0, 4, 0), // Target
    vec3(0, 1, 0)  // Up
  );

  lightingShader = Shader("shaders/lighting.vert", "shaders/lighting.frag");
  brickShader = Shader("shaders/brick.vert", "shaders/brick.frag");
  depthShader = Shader("shaders/depth.vert", "shaders/depth.frag");
  shadowTestShader = Shader("shaders/shadow_test.vert", "shaders/shadow_test.frag");

  initTextures();
  initShadows();

  // Room geometry
  Plane backWall = Plane(-4.0f, 6.0f, -4.0f, 8.0f, 6.0f, vec3(0, 0, 1));
  vector<GLfloat> backWallVertices = backWall.Triangles();
  vector<vec3> backWallNormals = backWall.Normals();
  vector<GLfloat> backWallUVs = backWall.UVs();

  Plane sideWall = Plane(4.0f, 6.0f, -4.0f, 8.0f, 6.0f, vec3(-1, 0, 0));
  vector<GLfloat> sideWallVertices = sideWall.Triangles();
  vector<vec3> sideWallNormals = sideWall.Normals();
  vector<GLfloat> sideWallUVs = sideWall.UVs();

  Plane floor = Plane(-4.0f, 0.0f, -4.0f, 8.0f, 8.0f, vec3(0, 1, 0));
  vector<GLfloat> floorVertices = floor.Triangles();
  vector<vec3> floorNormals = floor.Normals();
  vector<GLfloat> floorUVs = floor.UVs();

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

  vector<GLfloat> g_uv_buffer_data;
  g_uv_buffer_data.insert(g_uv_buffer_data.end(), backWallUVs.begin(), backWallUVs.end());
  g_uv_buffer_data.insert(g_uv_buffer_data.end(), sideWallUVs.begin(), sideWallUVs.end());
  g_uv_buffer_data.insert(g_uv_buffer_data.end(), floorUVs.begin(), floorUVs.end());

  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, g_uv_buffer_data.size() * sizeof(GLfloat), &g_uv_buffer_data[0], GL_STATIC_DRAW);

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

void setupLightingShaderUniforms() {
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
}

void setupPositionalUniforms(Shader shader) {
  glUniformMatrix4fv(shader.Uniform("Model"), 1, GL_FALSE, &camera.Model()[0][0]);
  glUniformMatrix4fv(shader.Uniform("View"), 1, GL_FALSE, &camera.View()[0][0]);
  glUniformMatrix4fv(shader.Uniform("Projection"), 1, GL_FALSE, &camera.Projection()[0][0]);

  glUniform3f(shader.Uniform("CameraPosition"), camera.Position().x, camera.Position().y, camera.Position().z);
}

mat4 shadowMatrix() {
  mat4 projection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
  mat4 view = lookAt(
    vec3(0.0f, 3.5f, -4.0f), // Light Pos
    vec3(0.0f, 0.0f, 0.0f), // Target (center)
    vec3(0.0f, 1.0f, 0.0f)
  );
  return projection * view;
}

void setupLightUniforms(Shader shader) {
  // Point Light (Candle)
  glUniform3f(shader.Uniform("lights[0].position"), 1.25f, 2.8f, -1.15f);
  glUniform3f(shader.Uniform("lights[0].ambient"), 0.2f, 0.2f, 0.2f);
  glUniform3f(shader.Uniform("lights[0].diffuse"), 0.5f, 0.5f, 0.5f);
  glUniform3f(shader.Uniform("lights[0].specular"), 1.0f, 1.0f, 1.0f);
  glUniform1f(shader.Uniform("lights[0].constant"), 1.0f);
  glUniform1f(shader.Uniform("lights[0].linear"), 0.09f);
  glUniform1f(shader.Uniform("lights[0].quadratic"), 0.032f);

  // Point Light (Torch)
  glUniform3f(shader.Uniform("lights[1].position"), 3.95f, 3.5f, 0.0f);
  glUniform3f(shader.Uniform("lights[1].ambient"), 0.2f, 0.2f, 0.2f);
  glUniform3f(shader.Uniform("lights[1].diffuse"), 0.5f, 0.5f, 0.5f);
  glUniform3f(shader.Uniform("lights[1].specular"), 1.0f, 1.0f, 1.0f);
  glUniform1f(shader.Uniform("lights[1].constant"), 1.0f);
  glUniform1f(shader.Uniform("lights[1].linear"), 0.09f);
  glUniform1f(shader.Uniform("lights[1].quadratic"), 0.032f);

  // Directional Light
  glUniform3f(shader.Uniform("dirLight.direction"), 0.0, 0.0, 1.0);
  glUniform3f(shader.Uniform("dirLight.ambient"), 0.2f, 0.2f, 0.2f);
  glUniform3f(shader.Uniform("dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
  glUniform3f(shader.Uniform("dirLight.specular"), 1.0f, 1.0f, 1.0f);

  // Shadow Map
  glUniformMatrix4fv(shader.Uniform("shadowMatrix"), 1, GL_FALSE, &shadowMatrix()[0][0]);
  glUniform1i(shader.Uniform("shadowMap"), 4);
}

void setupBrickUniforms() {
  setupPositionalUniforms(brickShader);

  // Brick Material Info
  glUniform1i(brickShader.Uniform("material.diffuse"), 0);
  glUniform1i(brickShader.Uniform("material.specular"), 1);
  glUniform1f(brickShader.Uniform("material.shininess"), 32.0f);

  setupLightUniforms(brickShader);
}

void setupHardwoodUniforms() {
  setupPositionalUniforms(brickShader);

  // Brick Material Info
  glUniform1i(brickShader.Uniform("material.diffuse"), 2);
  glUniform1i(brickShader.Uniform("material.specular"), 3);
  glUniform1f(brickShader.Uniform("material.shininess"), 32.0f);

  setupLightUniforms(brickShader);
}

void renderShadowMap() {
  glUseProgram(depthShader.Program());
  glUniformMatrix4fv(depthShader.Uniform("shadowMatrix"), 1, GL_FALSE, &shadowMatrix()[0][0]);
  glUniformMatrix4fv(depthShader.Uniform("model"), 1, GL_FALSE, &camera.Model()[0][0]);

  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 285);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void drawScene() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, brickDiffuse.id());

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, brickSpecular.id());

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, hardwoodDiffuse.id());

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, hardwoodSpecular.id());

  glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glCullFace(GL_FRONT);
    renderShadowMap();
    glCullFace(GL_BACK);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(brickShader.Program());
    setupBrickUniforms();
    glDrawArrays(GL_TRIANGLES, 0, 12);

    setupHardwoodUniforms();
    // glUseProgram(shadowTestShader.Program());
    // glUniformMatrix4fv(shadowTestShader.Uniform("MVP"), 1, GL_FALSE, &camera.MVP()[0][0]);
    // glUniform1i(shadowTestShader.Uniform("depthMap"), 4);
    glDrawArrays(GL_TRIANGLES, 12, 6);

    glUseProgram(lightingShader.Program());
    setupLightingShaderUniforms();
    glDrawArrays(GL_TRIANGLES, 18, 267);
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

  glGenFramebuffers(1, &depthMapFBO);

  glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LESS);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  initScene();
  render();

  return 0;
}