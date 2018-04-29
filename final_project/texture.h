#ifndef TEXTURE
#define TEXTURE

#include <GL/glew.h>
#include "lib/stb_image.h"

class Texture {
  const char* path;
  unsigned char* image;
  int width;
  int height;
  int comp;
  GLuint texture;

public:
  Texture(const char* filename);
  ~Texture();
  bool load();
  GLuint id();
};

#endif