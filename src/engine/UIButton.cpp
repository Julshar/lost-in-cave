#include "UIButton.h"
#include "MouseInput.h"

#undef DEBUG

std::list<UIButton*> UIButton::instances;
UIButton *UIButton::hoveredButton = nullptr;

UIButton::UIButton(UIObject *parent, glm::fvec2 size)
:
parent_(parent),
size_(size),

pressed_(false),
clicked_(false),
hovered_(false),
hidden_(false)
{
  calculateEdges();

  instances.push_front(this);
  instancePosition_ = (instances.begin());
}

UIButton::~UIButton()
{
  instances.erase(instancePosition_);
}

bool UIButton::isClicked()
{
  if (clicked_)
  {
    clicked_ = false;
    return true;
  }

  return false;
}

bool UIButton::isPressed()
{
  return pressed_;
}

bool UIButton::isHoveredOver()
{
  return hovered_;
}

void UIButton::hide()
{
  hidden_ = true;
}

void UIButton::show()
{
  hidden_ = false;
}

void UIButton::updateInstances()
{
  hoveredButton = getHoveredButton();

  if (hoveredButton != nullptr)
  {
    if (MouseInput::isLeftPressed())
    {
      hoveredButton->pressed_ = true;
    }
    else if (MouseInput::isLeftPressed() == false)
    {
      if (hoveredButton->pressed_ == true)
      {
        hoveredButton->clicked_ = true;
        hoveredButton->pressed_ = false;
      }
    }
  }
}

UIButton *UIButton::getHoveredButton()
{
  glm::fvec2 mousePos = MouseInput::getPosition();

  if (hoveredButton != nullptr)
  {
    if (hoveredButton->hidden_)
    {
      #ifdef DEBUG
      printf("UIButton::getHoveredButton(): Previously hovered button is now hidden.\n");
      #endif

      hoveredButton->pressed_ = false;
      hoveredButton->hovered_ = false;
    }
    else if (hoveredButton->isPointInsideButton(mousePos))
    {
      #ifdef DEBUG
      printf("UIButton::getHoveredButton(): Previously hovered button is still hovered.\n");
      #endif

      return hoveredButton;
    }
    else
    {
      #ifdef DEBUG
      printf("UIButton::getHoveredButton(): Previously hovered button is no longer hovered.\n");
      #endif

      hoveredButton->pressed_ = false;
      hoveredButton->hovered_ = false;
    }
  }

  for (UIButton *button : instances)
  {
    if (button->hidden_ == false && button->isPointInsideButton(mousePos))
    {
      #ifdef DEBUG
      printf("UIButton::getHoveredButton(): One of buttons is hovered for first frame is a row.\n");
      #endif

      button->hovered_ = true;
      return button;
    }
  }
  
  #ifdef DEBUG
  printf("UIButton::getHoveredButton(): None of buttons is hovered.\n");
  #endif

  return nullptr;
}

bool UIButton::isPointInsideButton(glm::fvec2 point)
{
  #ifdef DEBUG
  printf("UIButton::isPointInsideButton(): Mouse position: (%f, %f).\n", point.x, point.y);
  printf("UIButton::isPointInsideButton(): Button edges: (top, right, bottom, left).\n");
  printf("UIButton::isPointInsideButton(): Button edges: (%f, %f, %f, %f).\n", edges_.top, edges_.right, edges_.bottom, edges_.left);
  #endif

  if (point.x > edges_.left && point.x < edges_.right && point.y > edges_.top && point.y < edges_.bottom)
  {
    return true;
  }

  return false;
}

void UIButton::calculateEdges()
{ 
  edges_ =
  {
    parent_->position_.x + parent_->pivot_.x - (size_.x / 2),
    parent_->position_.x + parent_->pivot_.x + (size_.x / 2),
    parent_->position_.y + parent_->pivot_.y - (size_.y / 2),
    parent_->position_.y + parent_->pivot_.y + (size_.y / 2)
  };

  #ifdef DEBUG
  printf("UIButton::calculateEdges(): Calculated new button edges: (top, right, bottom, left).\n");
  printf("UIButton::calculateEdges(): Button edges: (%f, %f, %f, %f).\n", edges_.top, edges_.right, edges_.bottom, edges_.left);
  printf("UIButton::calculateEdges(): Parent position: (%f, %f).\n", parent_->position_.x, parent_->position_.y);
  printf("UIButton::calculateEdges(): Parent pivot: (%f, %f).\n", parent_->pivot_.x, parent_->pivot_.y);
  #endif
}