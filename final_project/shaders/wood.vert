#version 450 core

layout (location = 0) in vec3 vertexPosition_modelspace;
layout (location = 1) in vec2 vertexTextureCoords;
layout (location = 2) in vec3 vertexNormal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 shadowMatrix;

out vec3 FragPosition;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosition_LightSpace;

void main() {
  gl_Position = Projection * View * Model * vec4(vertexPosition_modelspace, 1.0);

  FragPosition = vec3(Model * vec4(vertexPosition_modelspace, 1.0));
  Normal = vertexNormal;
  TexCoords = vertexTextureCoords;
  FragPosition_LightSpace = shadowMatrix * vec4(FragPosition, 1.0);
}