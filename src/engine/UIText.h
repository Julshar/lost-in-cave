#pragma once

#include "TextFont.h"

#include "UIObject.h"

#include <string>
#include <list>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>

class UIObject;

class UIText
{
public:

  std::string content_;
  glm::fvec4 color_;
  glm::fvec2 positionOffset_;
  TextFont *font_;

  ~UIText();

  unsigned int getWidth();
  unsigned int getHeight();

  void hide();
  void show();

  void centerText();

private:

  friend class UITextRenderer;
  friend class UIObject;

  UIText(UIObject *parent, std::string content, TextFont *font, glm::fvec4 color = glm::vec4(1.0f));

  UIObject *parent_;

  bool hidden_;
};