#include "UIObject.h"

#include <cstdlib>

#undef DEBUG
#define WARNING

/*
* UILayer class
*/

std::vector<UILayer*> UILayer::instances;

/*
* depth - layer property that indicates rendering order.
* Layer with higher depth value will be rendered after (on top of)
* all other layers with lower depth value. This property also has to be
* unique across all existing layers. Request to create layer
* with depth equal to depth of an existing layer will result in failure
* and thus this function returning "1".
*/
int UILayer::addLayer(unsigned int depth, std::string name)
{
  #ifdef DEBUG
  printf("UILayer::addLayer(): Adding new layer '%s', depth = %u.\n", name.c_str(), depth);
  #endif

  std::vector<UILayer*>::iterator iter = instances.begin();

  for (iter; iter < instances.end(); iter++)
  {
    if ((*iter)->depth_ == depth)
    {
      #ifdef WARNING
      printf("UILayer::addLayer(): WARNING! Layer with depth %u already exists. Can't create another.\n", depth);
      #endif

      return 1;
    }
    else if ((*iter)->depth_ > depth)
    {
      #ifdef DEBUG
      printf("UILayer::addLayer(): Layer '%s' has depth %u. Placing current layer before it.\n", (*iter)->name_.c_str(), (*iter)->depth_);
      #endif

      break;
    }
  }

  if (iter == instances.end())
  {
    #ifdef DEBUG
    printf("UILayer::addLayer(): There is no layer that has greater depth than currently created.\n");
    #endif
  }

  UILayer* layer = new UILayer(depth, name);

  instances.insert(iter, layer);

  #ifdef DEBUG
  printf("UILayer::addLayer(): Layer successfully created.\n");
  #endif

  return 0;
}

UILayer* UILayer::getLayer(std::string name)
{
  for (UILayer *layer : instances)
  {
    if (layer->name_ == name)
    {
      return layer;
    }
  }

  #ifdef WARNING
  printf("UILayer::getLayer(): WARNING! No layer named '%s' found.\n", name.c_str());
  #endif

  return nullptr;
}

UILayer* UILayer::getLayer(unsigned int depth)
{
  for (UILayer *layer : instances)
  {
    if (layer->depth_ == depth)
    {
      return layer;
    }
  }

  #ifdef WARNING
  printf("UILayer::getLayer(): WARNING! No layer which depth is '%u' found.\n", depth);
  #endif

  return nullptr;
}

UILayer::UILayer(unsigned int depth, std::string name)
:
depth_(depth),
name_(name)
{

}

std::list<UIObject*>::iterator UILayer::addUIObject(UIObject *obj)
{
  UIObjects_.push_front(obj);
  return UIObjects_.begin();
}

std::list<UISprite*>::iterator UILayer::addUISprite(UISprite *sprite)
{
  UISprites_.push_front(sprite);
  return UISprites_.begin();
}

std::list<UIText*>::iterator UILayer::addUIText(UIText *text)
{
  UITexts_.push_front(text);
  return UITexts_.begin();
}

void UILayer::removeUIObject(std::list<UIObject*>::iterator it)
{
  UIObjects_.erase(it);
}

void UILayer::removeUISprite(std::list<UISprite*>::iterator it)
{
  UISprites_.erase(it);
}

void UILayer::removeUIText(std::list<UIText*>::iterator it)
{
  UITexts_.erase(it);
}

void UILayer::logExistingLayers()
{
  for (size_t i = 0; i < instances.size(); ++i)
  {
    #ifdef DEBUG
    printf("UILayer::logExistingLayers(): Layer #%u: depth = %u, name = %s.\n", i, instances[i]->depth_, instances[i]->name_.c_str());
    #endif
  }
}

std::string UILayer::getName()
{
  return name_;
}

/*
* UIObject class
*/

int UIObject::windowWidth;
int UIObject::windowHeight;

/*
* This function can be overriden by user to call it every frame
*/
void UIObject::update()
{

}

UIObject::UIObject()
:
button_(nullptr),
sprite_(nullptr),
text_(nullptr),

pivotPosition_(PivotPosition::TOP_LEFT),
pivot_(0.0f, 0.0f),
position_(0.0f, 0.0f),

layer_(nullptr),
hidden_(false)
{

}

UIObject::UIObject(UILayer *layer)
:
button_(nullptr),
sprite_(nullptr),
text_(nullptr),

pivotPosition_(PivotPosition::TOP_LEFT),
pivot_(0.0f, 0.0f),
position_(0.0f, 0.0f),

layer_(layer),
hidden_(false)
{
  if (layer == nullptr)
  {
    #ifdef WARNING
    printf("UIObject::UIObject(): WARNING! Passed layer is nullptr. Objects layer will remain 'nullptr'.\n");
    #endif
  }
  else
  {
    #ifdef DEBUG
    printf("UIObject::UIObject(): Creating UI object assigned to layer '%s'.\n", layer->getName().c_str());
    #endif

    objectPositionInLayer_ = layer->addUIObject(this);
  }
}

void UIObject::setLayer(UILayer *layer)
{
  if (layer == nullptr)
  {
    #ifdef WARNING
    printf("UIObject::setLayer(): WARNING! Passed layer is nullptr. Ignoring request to set layer.\n");
    #endif

    return;
  }

  if (this->layer_ != nullptr)
  {
    #ifdef DEBUG
    printf("UIObject::setLayer(): Object is already assigned to layer. Going to remove it from that layer.\n");
    #endif

    removeFromLayer();
  }

  #ifdef DEBUG
  printf("UIObject::setLayer(): Assigning object to new layer.\n");
  #endif

  addToLayer(layer);
}

void UIObject::removeFromLayer()
{
  if (layer_ == nullptr)
  {
    return;
  }

  layer_->removeUIObject(objectPositionInLayer_);

  if (sprite_ != nullptr)
  {
    layer_->removeUISprite(spritePositionInLayer_);
  }

  if (text_ != nullptr)
  {
    layer_->removeUIText(textPositionInLayer_);
  }
}

/*
* This function assumes that passed layer
* is a valid layer (not a 'nullptr').
*/
void UIObject::addToLayer(UILayer *layer)
{
  layer_ = layer;

  objectPositionInLayer_ = layer->addUIObject(this);

  if (sprite_ != nullptr)
  {
    spritePositionInLayer_ = layer_->addUISprite(sprite_);
  }

  if (text_ != nullptr)
  {
    textPositionInLayer_ = layer_->addUIText(text_);
  }
}

UIObject::~UIObject()
{
  removeFromLayer();

  if (sprite_ != nullptr)
  {
    delete sprite_;
  }

  if (text_ != nullptr)
  {
    delete text_;
  }

  if (button_ != nullptr)
  {
    delete button_;
  }
}

void UIObject::addButton(glm::fvec2 size)
{
  #ifdef DEBUG
  printf("UIObject::addButton(): Adding button module.\n");
  #endif

  button_ = new UIButton(this, size);
}

void UIObject::addSprite(glm::fvec2 size, Texture *texture)
{
  if (layer_ == nullptr)
  {
    #ifdef WARNING
    printf("UIObject::addSprite(): WARNING! Cannot add sprite when layer not set. Set layer first.\n");
    #endif

    return;
  }

  #ifdef DEBUG
  printf("UIObject::addSprite(): Adding sprite module.\n");
  #endif

  sprite_ = new UISprite(this, size, texture);
  spritePositionInLayer_ = layer_->addUISprite(sprite_);
}

void UIObject::addText(std::string content, TextFont *font, glm::fvec4 color)
{
  if (layer_ == nullptr)
  {
    #ifdef WARNING
    printf("UIObject::addText(): WARNING! Cannot add text when layer not set. Set layer first.\n");
    #endif

    return;
  }

  #ifdef DEBUG
  printf("UIObject::addText(): Adding text module.\n");
  #endif

  text_ = new UIText(this, content, font, color);
  textPositionInLayer_ = layer_->addUIText(text_);
}

void UIObject::hide()
{
  hidden_ = true;

  if (sprite_ != nullptr)
  {
    sprite_->hide();
  }

  if (text_ != nullptr)
  {
    text_->hide();
  }

  if (button_ != nullptr)
  {
    button_->hide();
  }
}

void UIObject::show()
{
  hidden_ = false;

  if (sprite_ != nullptr)
  {
    sprite_->show();
  }

  if (text_ != nullptr)
  {
    text_->show();
  }

  if (button_ != nullptr)
  {
    button_->show();
  }
}

void UIObject::setPivot(PivotPosition pivotPosition)
{
  pivotPosition_ = pivotPosition;

  updatePivot();

  if (button_ != nullptr)
  {
    button_->calculateEdges();
  }
}

int UIObject::getWindowWidth()
{
  return windowWidth;
}

int UIObject::getWindowHeight()
{
  return windowHeight;
}

void UIObject::updatePivot()
{
  switch (pivotPosition_)
  {
    case PivotPosition::TOP_LEFT:
    {
      pivot_ = {0.0f, 0.0f};
      break;
    }
    case PivotPosition::TOP_RIGHT:
    {
      pivot_ = {windowWidth, 0.0f};
      break;
    }
    case PivotPosition::BOTTOM_LEFT:
    {
      pivot_ = {0.0f, windowHeight};
      break;
    }
    case PivotPosition::BOTTOM_RIGHT:
    {
      pivot_ = {windowWidth, windowHeight};
      break;
    }
    case PivotPosition::MIDDLE:
    {
      pivot_ = {windowWidth / 2, windowHeight / 2};
      break;
    }
    case PivotPosition::TOP:
    {
      pivot_ = {windowWidth / 2, 0.0f};
      break;
    }
    case PivotPosition::LEFT:
    {
      pivot_ = {0.0f, windowHeight / 2};
      break;
    }
    case PivotPosition::BOTTOM:
    {
      pivot_ = {windowWidth / 2, windowHeight};
      break;
    }
    case PivotPosition::RIGHT:
    {
      pivot_ = {windowWidth, windowHeight / 2};
      break;
    }
  }
}

void UIObject::setPosition(glm::fvec2 position)
{
  position_ = position;

  if (button_ != nullptr)
  {
    button_->calculateEdges();
  }
}

void UIObject::updateOnWindowResize(int width, int height)
{
  UIObject::windowWidth = width;
  UIObject::windowHeight = height;

  for (UILayer *layer : UILayer::instances)
  {
    for (UIObject *obj : layer->UIObjects_)
    {
      obj->updatePivot();

      if (obj->button_ != nullptr)
      {
        obj->button_->calculateEdges();
      }

      if (obj->sprite_ != nullptr)
      {
        if (obj->sprite_->isWindowWidth_)
        {
          obj->sprite_->size_.x = windowWidth;
        }
        if (obj->sprite_->isWindowHeight_)
        {
          obj->sprite_->size_.x = windowHeight;
        }
      }
    }
  }
}