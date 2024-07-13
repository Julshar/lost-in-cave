#pragma once

#include "UIObject.h"
#include "Texture.h"

class MenuButton : public UIObject
{
public:

  MenuButton(glm::fvec2 position, std::string text, glm::fvec2 size = {500.0f, 150.0f});

  void highlight(bool light);

private:

  static glm::fvec4 spriteColorLight;
  static glm::fvec4 spriteColorDark;
  static glm::fvec4 textColorLight;
  static glm::fvec4 textColorDark;
};