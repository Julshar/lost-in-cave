#pragma once

#include "Window.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

class MouseInput
{
public:

  MouseInput(Window *window);
  ~MouseInput();

  static glm::dvec2 getPosition();
  static bool isLeftPressed();
  static bool isRightPressed();
  static bool isCursorInsideWindow();

  static int setCursorImage(unsigned char *data, int width, int height, int xhot, int yhot);
  static void restoreDefaultCursorImage();

private:

  static MouseInput *mouseInput;

  Window *window_;

  GLFWcursor *cursor_;

  /*
  * TODO:
  *
  * Event based approach would probably be better in mouse input case
  * 
  * currently it can happen that on fast click
  * mouse button will be pressed and released
  * or released and pressed again
  * in single frame (thus input will be missed)
  */

  static glm::dvec2 cursorPosition;

  static bool leftPressed;
  static bool rightPressed;
  static bool isInsideWindow;

  void init();

  static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
  static void cursorEnterCallback(GLFWwindow* window, int entered);
  static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};