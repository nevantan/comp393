#version 450 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D normal;
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

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPos);

void main() {
  vec3 n = normalize(Normal);
  vec3 viewDir = normalize(CameraPosition - FragPosition);

  vec3 res = CalcDirLight(dirLight, n, viewDir);
  for(int i = 0; i < NR_POINT_LIGHTS; i++) {
    res += CalcPointLight(lights[i], n, FragPosition, viewDir);
  }

  FragColor = vec4(res, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-light.direction);
  
  // Diffuse
  float diff = max(dot(normalize(texture(material.normal, TexCoords*2).rgb), lightDir), 0.0);

  // Specular
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  // Calculate shadow
  float shadow = 1.0 - ShadowCalculation(FragPosition_LightSpace);

  // Calculate components
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords*2));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords*2)) * shadow;
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords*2)) * shadow;

  // Return combined components from this light source
  return ambient + diffuse + specular;
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fragPos);

  // Diffuse
  float diff = max(dot(normalize(texture(material.normal, TexCoords*2).rgb * 2.0), lightDir), 0.0);

  // Specular
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  // Attenuation
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  // Calculate shadow
  float shadow = 1.0 - ShadowCalculation(FragPosition_LightSpace);

  // Calculate components
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords*2.0)) * attenuation;
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords*2.0)) * attenuation * shadow;
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords*2.0)) * attenuation * shadow;

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