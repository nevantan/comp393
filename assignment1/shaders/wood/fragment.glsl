#version 450 core

in vec2 xyPosition;
in vec3 fragmentColor;

out vec3 color;

void main() {
  vec3 woodColor = vec3(0.808, 0.733, 0.619);
  vec3 mortarColor = vec3(0.85, 0.86, 0.84);
  vec2 woodSize = vec2(1.8, 0.9);
  vec2 brickPct = vec2(0.9, 0.9);

  vec2 position = xyPosition / woodSize;

  if(fract(position.y * 0.5) > 0.5) position.x += 0.5;

  position = fract(position);

  vec2 useBrick = step(position, brickPct);

  color = mix(mortarColor, woodColor, useBrick.x * useBrick.y);
}