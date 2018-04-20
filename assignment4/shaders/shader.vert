#version 450 core

layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 MVP;
uniform float MaxHeight;

out vec3 vertColor;

void main() {
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);

  vec3 waterColor = vec3(0.42, 0.56, 0.68);
  vec3 sandColor = vec3(0.91, 0.77, 0.28);
  vec3 grassColor = vec3(0.61, 0.77, 0.24);
  vec3 mountainColor = vec3(0.59, 0.59, 0.59);
  vec3 snowColor = vec3(1.0, 1.0, 1.0);
  if(vertexPosition_modelspace.y <= MaxHeight * 0.1) vertColor = waterColor;
  else if(vertexPosition_modelspace.y <= MaxHeight * 0.15) vertColor = sandColor;
  else if(vertexPosition_modelspace.y <= MaxHeight * 0.7) vertColor = grassColor;
  else if(vertexPosition_modelspace.y <= MaxHeight * 0.9) vertColor = mountainColor;
  else vertColor = snowColor;
}