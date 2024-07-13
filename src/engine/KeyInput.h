#pragma once

#include <GLFW/glfw3.h>

#include <vector>
#include <map>

class KeyInput
{
public:

  KeyInput(std::vector <int> keysToTrack, bool enabled = true);
  ~KeyInput();

  static void setKeyCallback(GLFWwindow* window);

  void enable(bool enable);
  bool isKeyPressed(int key);

private:

  std::map <int, bool> keys_;
  bool isEnabled_;
  int instanceId_;

  static std::vector <KeyInput*> instances;
  static int idSeed;

  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};