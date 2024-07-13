#include "KeyInput.h"

std::vector<KeyInput*> KeyInput::instances;
int KeyInput::idSeed = 1;

KeyInput::KeyInput(std::vector <int> keysToTrack, bool enabled)
:
isEnabled_(enabled)
{
  for (int key : keysToTrack)
  {
    keys_[key] = false;
  }

  instanceId_ = idSeed;
  idSeed++;

  KeyInput::instances.push_back(this);
}

KeyInput::~KeyInput()
{
  for (size_t i = 0; i < KeyInput::instances.size(); ++i)
  {
    if (KeyInput::instances[i] -> instanceId_ == this -> instanceId_)
    {
      KeyInput::instances.erase(KeyInput::instances.begin() + i);

      break;
    }
  }
}

bool KeyInput::isKeyPressed(int key)
{
  if (isEnabled_)
  {
    std::map<int, bool>::iterator it = keys_.find(key);

    if (it != keys_.end())
    {
      return keys_[key];
    }
  }

  return false;
}

void KeyInput::enable(bool enable)
{
  isEnabled_ = enable;
}

void KeyInput::setKeyCallback(GLFWwindow* window)
{
  glfwSetKeyCallback(window, KeyInput::keyCallback);
}

void KeyInput::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  for (KeyInput* keyInput : instances)
  {
    std::map<int, bool>::iterator it = keyInput -> keys_.find(key);

    if (it != keyInput -> keys_.end())
    {
      keyInput -> keys_[key] = (action != GLFW_RELEASE);
    }
  }
}