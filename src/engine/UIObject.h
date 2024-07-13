#pragma once

#include "UIButton.h"
#include "UISprite.h"
#include "UIText.h"

#include <glm/glm.hpp>

#include <list>
#include <string>
#include <vector>

/*
* Forward declarations
*/
class UIButton;
class UISprite;
class UIText;
class UIObject;

class UILayer
{
public:

  static int addLayer(unsigned int depth, std::string name);
  static UILayer* getLayer(std::string name);
  static UILayer* getLayer(unsigned int depth);
  static void logExistingLayers();

  std::string getName();

private:

  friend class UIObject;
  friend class Engine;
  
  static std::vector<UILayer*> instances;

  unsigned int depth_;
  std::string name_;

  std::list<UIObject*> UIObjects_;
  std::list<UISprite*> UISprites_;
  std::list<UIText*> UITexts_;

  UILayer(unsigned int depth, std::string name);

  std::list<UIObject*>::iterator addUIObject(UIObject *obj);
  std::list<UISprite*>::iterator addUISprite(UISprite *sprite);
  std::list<UIText*>::iterator addUIText(UIText *text);

  void removeUIObject(std::list<UIObject*>::iterator it);
  void removeUISprite(std::list<UISprite*>::iterator it);
  void removeUIText(std::list<UIText*>::iterator it);
};

enum class PivotPosition
{
  MIDDLE,
  TOP,
  LEFT,
  BOTTOM,
  RIGHT,
  TOP_LEFT,
  BOTTOM_LEFT,
  BOTTOM_RIGHT,
  TOP_RIGHT
};

class UIObject
{
public:

  UIObject(UILayer *layer);
  UIObject();
  ~UIObject();

  glm::fvec2 position_;
  glm::fvec2 pivot_;

  UIButton *button_;
  UISprite *sprite_;
  UIText *text_;

  /*
  * Override this function in derived class
  * to update derived object every game tick
  */
  virtual void update();

  void addButton(glm::fvec2 size);
  void addSprite(glm::fvec2 size, Texture *texture = nullptr);
  void addText(std::string content, TextFont *font, glm::fvec4 color = glm::vec4(1.0f));

  void setLayer(UILayer *layer);
  void setPivot(PivotPosition pivotPosition);

  void setPosition(glm::fvec2 position);

  void hide();
  void show();

  int getWindowWidth();
  int getWindowHeight();

private:
  
  friend class Engine;

  UILayer *layer_;
  std::list<UIObject*>::iterator objectPositionInLayer_;
  std::list<UISprite*>::iterator spritePositionInLayer_;
  std::list<UIText*>::iterator textPositionInLayer_;

  PivotPosition pivotPosition_;

  static int windowWidth;
  static int windowHeight;

  bool hidden_;

  void removeFromLayer();
  void addToLayer(UILayer *layer);
  void updatePivot();

  static void updateOnWindowResize(int width, int height);

};