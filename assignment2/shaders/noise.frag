#version 450 core

in vec4 amp;
in vec4 freq;
in vec2 mouse;

out vec4 color;

/*
 * 3D Functions
 */

float rand3D(vec3 pt) {
  return fract(sin(dot(pt, vec3(12.9898, 78.233, 35.482)))*43758.5453123);
}

float interpolateNoise3D(vec3 pt, bool useSmooth) {
  vec3 i = floor(pt);
  vec3 f = fract(pt);

  float v1 = rand3D(i + vec3(0.0, 0.0, 0.0));
  float v2 = rand3D(i + vec3(1.0, 0.0, 0.0));
  float v3 = rand3D(i + vec3(0.0, 1.0, 0.0));
  float v4 = rand3D(i + vec3(1.0, 1.0, 0.0));

  float v5 = rand3D(i + vec3(0.0, 0.0, 1.0));
  float v6 = rand3D(i + vec3(1.0, 0.0, 1.0));
  float v7 = rand3D(i + vec3(0.0, 1.0, 1.0));
  float v8 = rand3D(i + vec3(1.0, 1.0, 1.0));

  vec3 u = smoothstep(0.0, 1.0, f);

  float back = mix(mix(v1, v2, u.x), mix(v3, v4, u.x), u.y);
  float front = mix(mix(v5, v6, u.x), mix(v7, v8, u.x), u.y);

  return mix(back, front, u.z);
}

float makeNoise3D(vec3 pt, float size) {
  float v = 0.0;
  float is = size;

  while(size >= 1) {
    v += interpolateNoise3D(pt / vec3(size), false) * size;
    size /= 2.0;
  }

  return(0.5 * v / is);
}

void main() {
  float mAvg = (mouse.x + mouse.y) / 2.0;
  vec3 st = vec3(gl_FragCoord.xy, mAvg / 4.0);
  float size = 64;
  color = vec4(vec3(makeNoise3D(st, size)), 0.3);
}