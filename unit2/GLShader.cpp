#include "GLShader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

string readFile(const char *filepath) {
  string content;
  ifstream fileStream;
  fileStream.open(filepath);

  if(!fileStream) {
    cerr << "Could not read file " << filepath << ". File does not exist." << endl;
    return "Fail";
  }

  string line = "";
  while(!fileStream.eof()) {
    getline(fileStream, line);
    content.append(line + "\n");
  }

  fileStream.close();
  return content;
}

GLuint LoadShader(const char *vertex_path, const char *fragment_path) {
  GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  // Get sources from file
  string vertShaderStr = readFile(vertex_path);
  string fragShaderStr = readFile(fragment_path);
  const char *vertShaderSrc = vertShaderStr.c_str();
  const char *fragShaderSrc = fragShaderStr.c_str();

  GLint result = GL_FALSE;
  int logLength;

  // Compile vertex shader
  cout << "Compiling vertex shader." << endl;
  glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
  glCompileShader(vertShader);

  // Check vertex shader
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
  vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
  glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
  cout << &vertShaderError[0] << endl;

  // Compile fragment shader
  cout << "Compiling fragment shader." << endl;
  glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
  glCompileShader(fragShader);

  // Check fragment shader
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
  vector<GLchar> fragShaderError((logLength > 1) ? logLength : 1);
  glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
  cout << &fragShaderError[0] << endl;

  // Link program
  cout << "Linking program" << endl;
  GLuint program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);

  // Check program
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
  vector<char> programError((logLength > 1) ? logLength : 1);
  glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
  cout << &programError[0] << endl;

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  return program;
}