#version 450 core

in vec2 texPosition;

uniform vec3 WoodColor;
uniform vec3 SpacerColor;
uniform vec2 WoodSize;
uniform vec2 WoodPct;

out vec4 color;

void main() {
  vec2 position = texPosition / WoodSize;

  if(fract(position.y * 0.5) > 0.5) position.x += 0.5;

  position = fract(position);

  vec2 useWood = step(position, WoodPct);

  color = vec4(mix(SpacerColor, WoodColor, useWood.x * useWood.y), 1.0);
}