#version 450 core

in vec2 texPosition;
in vec3 fragmentColor;

uniform vec3 BrickColor;
uniform vec3 MortarColor;
uniform vec2 BrickSize;
uniform vec2 BrickPct;

out vec4 color;

void main() {
  vec2 position = texPosition / BrickSize;

  if(fract(position.y * 0.5) > 0.5) position.x += 0.5;

  position = fract(position);

  vec2 useBrick = step(position, BrickPct);

  color = vec4(mix(MortarColor, BrickColor, useBrick.x * useBrick.y), 1.0);
}