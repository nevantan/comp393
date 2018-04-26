#version 450 core

in vec2 UV;

out vec3 color;

uniform sampler2D Picture;

void main() {
  color = texture(Picture, UV).rgb * vec3(1.2, 0.9, 0.9);
}