#include "shader.h"
using namespace std;

Shader::Shader(string vert, string frag) {
  vertShader = glCreateShader(GL_VERTEX_SHADER);
  fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  string vertShaderStr = readFile(vert);
  string fragShaderStr = readFile(frag);

  compileShader(vertShader, vertShaderStr.c_str());
  verifyShader(vertShader);

  compileShader(fragShader, fragShaderStr.c_str());
  verifyShader(fragShader);
}

GLuint Shader::Program() {
  GLuint program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);

  return program;
}

Shader::~Shader() {
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
}

// Private
string Shader::readFile(string filepath) {
  string content;
  ifstream file(filepath, ios::in);

  if(!file.is_open()) {
    cerr << "Could not read file " << filepath << ". File does not exist." << endl;
    return "";
  }

  string line = "";
  while(!file.eof()) {
    getline(file, line);
    content.append(line + "\n");
  }

  file.close();

  return content;
}

void Shader::compileShader(GLuint& shader, const char *shaderSrc) {
  glShaderSource(shader, 1, &shaderSrc, NULL);
  glCompileShader(shader);
}

void Shader::verifyShader(GLuint& shader) {
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
  vector<char> shaderError((logLength > 1) ? logLength : 1);
  glGetShaderInfoLog(shader, logLength, NULL, &shaderError[0]);
  cout << &shaderError[0] << endl;
}