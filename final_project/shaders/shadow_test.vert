#version 450 core
layout (location = 0) in vec3 vertexPosition_modelspace;
layout (location = 1) in vec2 vertexTextureCoords;

uniform mat4 MVP;

out vec2 TexCoords;

void main() {
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);

  TexCoords = vertexTextureCoords;
}