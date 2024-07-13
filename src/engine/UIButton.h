#pragma once

#include "Texture.h"
#include "UIText.h"

#include "UIObject.h"

#include <glm/glm.hpp>

#include <vector>
#include <list>

class UIObject;

class UIButton
{
public:

  ~UIButton();

  bool isClicked();
  bool isPressed();
  bool isHoveredOver();

  void hide();
  void show();

private:

  struct Edges
  {
    float left;
    float right;
    float top;
    float bottom;
  };

  friend class Engine;
  friend class UIObject;

  UIButton(UIObject *parent, glm::fvec2 size);

  static std::list<UIButton*> instances;
  std::list<UIButton*>::iterator instancePosition_;

  static UIButton *hoveredButton;

  UIObject *parent_;

  Edges edges_;

  bool clicked_;
  bool pressed_;
  bool hovered_;
  bool hidden_;

  glm::fvec2 size_;

  void calculateEdges();

  static void updateInstances();
  static UIButton *getHoveredButton();
  bool isPointInsideButton(glm::fvec2 point);
};