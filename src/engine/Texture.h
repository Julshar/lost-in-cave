#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>

class Texture
{
public:
  
  Texture(const char *path, std::string name);

  int width_;
  int height_;
  int channelCount_;

  unsigned int textureId_;

  std::string name_;

  static Texture *getInstance(std::string name);
  static void loadTexture(const char *path, std::string name);
  static void unloadAllTextures();

  void bind(int textureUnit);

private:

  Texture(std::string name);

  static std::vector<Texture *> instances;

  int init(const char *path);
  void deinitialize();
  
};