#pragma once

#include "Texture.h"
#include "Animation.h"

#include "UIObject.h"

#include <glm/glm.hpp>

#include <list>

class UIObject;

class UISprite
{
public:

  ~UISprite();

  glm::fvec2 size_;
  glm::fvec4 color_;
  float rotation_;

  void hide();
  void show();

  void setSizeToWindowWidth();
  void setSizeToWindowHeight();

  void addAnimation(Animation *animation);

private:

  friend class SpriteRenderer;
  friend class UIObject;

  UISprite(UIObject *parent, glm::fvec2 size, Texture *texture = nullptr);

  UIObject *parent_;

  bool hidden_;
  bool isWindowHeight_;
  bool isWindowWidth_;

  Texture *texture_;
  Animation *animation_;
};