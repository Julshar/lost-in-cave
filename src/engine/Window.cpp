#include "Window.h"
#include "FileManager.h"

#include <stdio.h>
#include <string>

#undef DEBUG

Window::Window()
{
  #ifdef DEBUG
  printf("Window::Window(): Creating window.\n");
  #endif

  std::string param;

  param = FileManager::readParameter("Window_width");
  width_ = std::stoi(param);

  param = FileManager::readParameter("Window_height");
  height_ = std::stoi(param);

  param = FileManager::readParameter("OpenGL_version_major");
  opengl_version_major_ = std::stoi(param);

  param = FileManager::readParameter("OpenGL_version_minor");
  opengl_version_minor_ = std::stoi(param);

  param = FileManager::readParameter("fullscreen");
  fullscreen_ = std::stoi(param);
}

Window::~Window()
{

}

GLFWwindow *Window::getHandle()
{
  if (NULL == handle_)
  {
    printf("Window::getHandle(): WARNING! Window handle is NULL.\n");
  }

  return handle_;
}

int Window::getWidth()
{
  return width_;
}

int Window::getHeight()
{
  return height_;
}
  
void Window::setSize(int width, int height)
{
  width_ = width;
  height_ = height;
}

int Window::init()
{
  if (!glfwInit())
  {
    printf("Window::init(): ERROR! Failed to initialize GLFW.\n");

    return 1;
  }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_version_major_);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_version_minor_);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef DEBUG
  printf("Window::init(): Initializing window with size %ix%i\n", width_, height_);
  printf("Window::init(): OpenGL version set to %i.%i\n", opengl_version_major_, opengl_version_minor_);
  #endif

  if (fullscreen_)
  {
    #ifdef DEBUG
    printf("Window::init(): Fullscreen mode on.\n");
    #endif

	  handle_ = glfwCreateWindow(width_, height_, "WINDOW TITLE", glfwGetPrimaryMonitor(), nullptr);
  }
  else
  {
    #ifdef DEBUG
    printf("Window::init(): Fullscreen mode off.\n");
    #endif
    
    handle_ = glfwCreateWindow(width_, height_, "WINDOW TITLE", nullptr, nullptr);
  }

  if (handle_ == NULL)
  {
    printf("Window::init(): ERROR! Failed to initialize window.\n");

    glfwTerminate();
    return 1;
  }

	glfwMakeContextCurrent(handle_);

  return 0;
}