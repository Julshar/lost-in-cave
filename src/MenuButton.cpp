#include "MenuButton.h"

#include <iostream>

glm::fvec4 MenuButton::spriteColorLight = {1.0f, 1.0f, 1.0f, 1.0f};
glm::fvec4 MenuButton::spriteColorDark = {0.5f, 0.5f, 0.5f, 1.0f};
glm::fvec4 MenuButton::textColorLight = {1.0f, 0.4f, 1.0f, 1.0f};
glm::fvec4 MenuButton::textColorDark = {0.7f, 0.2f, 0.7f, 1.0f};

MenuButton::MenuButton(glm::fvec2 position, std::string text, glm::fvec2 size) : UIObject(UILayer::getLayer("buttons"))
{
  addText(text, TextFont::getInstance("pixel_M"), textColorDark);
  text_->centerText();

  addSprite(size, Texture::getInstance("button"));
  sprite_->color_ = spriteColorDark;

  setPivot(PivotPosition::MIDDLE);
  setPosition(position);
  
  addButton(size);

  hide();
}

void MenuButton::highlight(bool light)
{
  if (light)
  {
    sprite_->color_ = spriteColorLight;
    text_->color_ = textColorLight;
  }
  else
  {
    sprite_->color_ = spriteColorDark;
    text_->color_ = textColorDark;
  }
}