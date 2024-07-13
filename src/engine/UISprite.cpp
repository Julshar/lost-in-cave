#include "UISprite.h"

#include <cstdlib>

UISprite::UISprite(UIObject *parent, glm::fvec2 size, Texture *texture)
:
parent_(parent),
size_(size),
texture_(texture),
hidden_(false),
isWindowHeight_(false),
isWindowWidth_(false),
animation_(nullptr),
color_({1.0f, 1.0f, 1.0f, 1.0f}),
rotation_(0.0f)
{

}

UISprite::~UISprite()
{

}

void UISprite::hide()
{
  hidden_ = true;
}

void UISprite::show()
{
  hidden_ = false;
}

void UISprite::setSizeToWindowWidth()
{
  isWindowWidth_ = true;

  size_.x = parent_->getWindowWidth();
}

void UISprite::setSizeToWindowHeight()
{
  isWindowHeight_ = true;

  size_.y = parent_->getWindowHeight();
}

void UISprite::addAnimation(Animation *animation)
{
  animation_ = animation;
}