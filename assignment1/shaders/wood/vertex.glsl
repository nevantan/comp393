#version 450 core

layout(location = 0) in vec3 vertexPosition_modelspace;

out vec2 texPosition;

uniform mat4 MVP;
uniform vec3 Plane;

void main() {
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

  if(Plane.x == 1.0 && Plane.y == 1.0) {
    texPosition = vertexPosition_modelspace.xy;
  } else if(Plane.x == 1.0 && Plane.z == 1.0) {
    texPosition = vertexPosition_modelspace.xz;
  } else {
    texPosition = vertexPosition_modelspace.zy;
  }
}