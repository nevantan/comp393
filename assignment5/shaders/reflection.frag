#version 450 core

in vec2 UV;

out vec3 color;

uniform sampler2D Picture;
uniform sampler2D NormalMap;

void main() {
  vec3 lightPos = vec3(-2.0, 4.0, 0.0);
  vec3 lightColor = vec3(1.0, 1.0, 1.0);

  vec3 n = normalize(texture2D(NormalMap, UV).rgb) * 2.0;
  vec3 l = normalize(lightPos);

  float cosTheta = clamp(dot(n, l), 0, 1);

  color = texture(Picture, UV).rgb * lightColor * cosTheta * 2.0;
}