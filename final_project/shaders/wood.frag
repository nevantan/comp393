#version 450 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};
#define NR_POINT_LIGHTS 2

struct DirLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 FragPosition;
in vec2 TexCoords;
in vec3 Normal;
in vec4 FragPosition_LightSpace;

uniform Light lights[NR_POINT_LIGHTS];
uniform DirLight dirLight;
uniform vec3 CameraPosition;
uniform Material material;
uniform sampler2D shadowMap;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 color);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color);
float ShadowCalculation(vec4 fragPos);

float random(vec2 st) {
  return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float noise(vec2 st) {
  vec2 i = floor(st);
  vec2 f = fract(st);
  vec2 u = f*f*(3.0-2.0*f);
  return mix(
    mix(
      random(i + vec2(0.0, 0.0)),
      random(i + vec2(1.0, 0.0)),
      u.x
    ),
    mix(
      random(i + vec2(0.0, 1.0)),
      random(i + vec2(1.0, 1.0)),
      u.x
    ),
    u.y
  );
}

mat2 rotate2d(float angle) {
  return mat2(
    cos(angle), -sin(angle),
    sin(angle), cos(angle)
  );
}

float lines(vec2 pos, float b) {
  float scale = 10.0;
  pos *= scale;
  return smoothstep(0.0, 0.5 + b * 0.5, abs((sin(pos.x*3.1415) + b*2.0)) * 0.5);
}

void main() {
  vec3 n = normalize(Normal);
  vec3 viewDir = normalize(CameraPosition - FragPosition);

  // Calculate pattern
  vec2 u_resolution = vec2(196, 196);
  vec2 st = gl_FragCoord.xy/u_resolution.xy;
  st.y *= u_resolution.y / u_resolution.x;

  vec2 pos = st.yx * vec2(5.0, 1.0);
  float pattern = pos.x;
  pos = rotate2d(noise(pos)) * pos;
  pattern = lines(pos, 0.98);
  vec3 color = vec3(pattern) * vec3(0.568, 0.308, 0.1) / 5;

  vec3 res = CalcDirLight(dirLight, n, viewDir, color);
  for(int i = 0; i < NR_POINT_LIGHTS; i++) {
    res += CalcPointLight(lights[i], n, FragPosition, viewDir, color);
  }

  FragColor = vec4(res, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 color) {
  vec3 lightDir = normalize(-light.direction);
  
  // Diffuse
  float diff = max(dot(normal, lightDir), 0.0);

  // Specular
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  // Calculate shadow
  float shadow = 1.0 - ShadowCalculation(FragPosition_LightSpace);

  // Calculate components
  vec3 ambient = light.ambient * color;
  vec3 diffuse = light.diffuse * diff * color * shadow;
  vec3 specular = light.specular * spec * color * shadow;

  // Return combined components from this light source
  return ambient + diffuse + specular;
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color) {
  vec3 lightDir = normalize(light.position - fragPos);

  // Diffuse
  float diff = max(dot(normal, lightDir), 0.0);

  // Specular
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  // Attenuation
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  // Calculate shadow
  float shadow = 1.0 - ShadowCalculation(FragPosition_LightSpace);

  // Calculate components
  vec3 ambient = light.ambient * color * attenuation;
  vec3 diffuse = light.diffuse * diff * color * attenuation * shadow;
  vec3 specular = light.specular * spec * color * attenuation * shadow;

  // Return combined components from this light source
  return ambient + diffuse + specular;
}

float ShadowCalculation(vec4 fragPosLightSpace) {
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;

  float closestDepth = texture(shadowMap, projCoords.xy).r;
  float currentDepth = projCoords.z;

  float bias = 0.05;
  float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

  if(projCoords.z > 1.0) shadow = 0.0;

  return shadow;
}