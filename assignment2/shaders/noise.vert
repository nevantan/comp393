#version 450 core

layout(location = 0) in vec3 vertexPosition_modelspace;

out vec4 amp;
out vec4 freq;
out vec2 mouse;

uniform mat4 MVP;
uniform vec2 Mouse;

void main() {
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
  amp = vec4(0.5, 1.0, 0.8, 1.0);
  freq = vec4(2.0, 2.0, 2.0, 2.0);
  mouse = Mouse;
}