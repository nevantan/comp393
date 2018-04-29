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
};

struct DirLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 FragPosition;
in vec2 TexCoords;
in vec3 Normal;

uniform Light light;
uniform vec3 CameraPosition;
uniform Material material;

out vec4 FragColor;

void main() {
  // Calculate Ambient Component
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords*2.0));

  // Calculate Diffuse Component
  vec3 n = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPosition);
  float diff = max(dot(n, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords*2.0));

  // Calculate Specular Component
  vec3 viewDir = normalize(CameraPosition - FragPosition);
  vec3 reflectDir = reflect(-lightDir, n);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords*2.0));

  // Calculate Final Fragment Color
  FragColor = vec4(ambient + diffuse + specular, 1.0);
}