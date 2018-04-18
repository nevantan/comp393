#version 450 core

in vec2 UV;

out vec4 color;

uniform sampler2D Picture;

void main() {
  vec2 fuzz = fwidth(UV) * 1.0;
  vec2 checkPos = smoothstep(0.0, 1.0, UV);
  vec2 p1 = smoothstep(vec2(0.015), fuzz * vec2(0.015), checkPos);
  vec2 p2 = smoothstep(vec2(0.985), 1.0 - (fuzz * vec2(0.015)), checkPos);
  color = vec4(texture(Picture, UV).rgb, (1.0 - p1.x - p1.y - p2.x - p2.y));
}