#version 450 core

in vec2 TexCoords;

uniform sampler2D depthMap;

out vec4 FragColor;

void main() {
  float depthValue = texture(depthMap, TexCoords).r;
  FragColor = vec4(vec3(depthValue), 1.0);
}