#version 450 core

layout (location = 0) in vec3 vertexPosition_modelspace;
layout (location = 2) in vec3 vertexNormal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 FragPosition;
out vec3 Normal;

void main() {
  gl_Position = Projection * View * Model * vec4(vertexPosition_modelspace, 1.0);

  FragPosition = vec3(Model * vec4(vertexPosition_modelspace, 1.0));
  Normal = vertexNormal;
}