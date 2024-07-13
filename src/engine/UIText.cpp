#include "UIText.h"

UIText::UIText(UIObject *parent, std::string content, TextFont *font, glm::fvec4 color)
:
parent_(parent),
content_(content),
font_(font),
color_(color),

hidden_(false),
positionOffset_(0.0f, 0.0f)
{

}

UIText::~UIText()
{

}

unsigned int UIText::getWidth()
{
  return font_->getTextWidth(&content_);
}

unsigned int UIText::getHeight()
{
  return font_->getTextHeight();
}

void UIText::hide()
{
  hidden_ = true;
}

void UIText::show()
{
  hidden_ = false;
}

void UIText::centerText()
{
  unsigned int textWidth = getWidth();
  unsigned int textHeight = getHeight();

  positionOffset_ =
  {
    (textWidth / 2) * -1.0f,
    (textHeight / 2) * -1.0f
  };
}