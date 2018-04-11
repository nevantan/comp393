#version 450 core

in vec2 UV;

uniform sampler2D Picture;

out vec4 color;

void main() {
  color = texture(Picture, UV);
}