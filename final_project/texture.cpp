#include "texture.h"

Texture::Texture(const char* filename) : path(filename) {
}

Texture::~Texture() {
  stbi_image_free(image);
}

bool Texture::load() {
  stbi_set_flip_vertically_on_load(true);
  image = stbi_load(
    path,
    &width,
    &height,
    &comp,
    STBI_rgb
  );
  if(!image) return false;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  return true;
}

GLuint Texture::id() {
  return texture;
}