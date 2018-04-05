#ifndef SHADER
#define SHADER

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class Shader {
  GLuint vertShader;
  GLuint fragShader;
  GLint result;
  int logLength;

  std::string readFile(std::string filepath);
  void compileShader(GLuint& shader, const char *shaderSrc);
  void verifyShader(GLuint& shader);

  public:
    Shader(std::string vert, std::string frag);
    ~Shader();
    GLuint Program();
};

#endif