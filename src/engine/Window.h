#pragma once

#include <GLFW/glfw3.h>

class Window
{
public:

  Window();
  ~Window();

  int init();

  GLFWwindow *getHandle();

  int getWidth();
  int getHeight();

  void setSize(int width, int height);

private:

  GLFWwindow *handle_;

  int width_;
  int height_;

  int opengl_version_major_;
  int opengl_version_minor_;

  bool fullscreen_;
};