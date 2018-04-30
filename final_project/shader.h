#ifndef SHADER
#define SHADER

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

class Shader {
  GLuint vertShader;
  GLuint geometryShader;
  GLuint fragShader;
  GLuint progID;
  GLint result;
  int logLength;
  std::map<std::string, GLuint> uniforms;

  std::string readFile(std::string filepath);
  void compileShader(GLuint& shader, const char *shaderSrc);
  void verifyShader(GLuint& shader);

  public:
    Shader();
    Shader(std::string vert, std::string frag);
    Shader(std::string vert, std::string geometry, std::string frag);
    ~Shader();
    GLuint Program();
    GLuint Uniform(std::string name);
};

#endif