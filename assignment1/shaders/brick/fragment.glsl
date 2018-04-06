#version 450 core

in vec2 xyPosition;
in vec3 fragmentColor;

out vec3 color;

void main() {
  vec3 brickColor = vec3(1.0, 0.3, 0.3);
  vec3 mortarColor = vec3(0.85, 0.86, 0.84);
  vec2 brickSize = vec2(1.8, 0.9);
  vec2 brickPct = vec2(0.9, 0.9);

  vec2 position = xyPosition / brickSize;

  if(fract(position.y * 0.5) > 0.5) position.x += 0.5;

  position = fract(position);

  vec2 useBrick = step(position, brickPct);

  color = mix(mortarColor, brickColor, useBrick.x * useBrick.y);
}