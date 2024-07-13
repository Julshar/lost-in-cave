#include "Sprite.h"

#undef DEBUG

/*
* SPRITE LAYER CLASS
*/

std::vector<SpriteLayer*> SpriteLayer::instances;

int SpriteLayer::addLayer(unsigned int depth, std::string name)
{
  #ifdef DEBUG
  printf("SpriteLayer::addLayer(): Adding new layer '%s', depth = %u.\n", name.c_str(), depth);
  #endif

  std::vector<SpriteLayer*>::iterator iter = instances.begin();

  for (iter; iter < instances.end(); iter++)
  {
    if ((*iter)->depth_ == depth)
    {
      printf("SpriteLayer::addLayer(): WARNING! Layer with depth %u already exists. Can't create another.\n", depth);

      return 1;
    }
    else if ((*iter)->depth_ > depth)
    {
      #ifdef DEBUG
      printf("SpriteLayer::addLayer(): Layer '%s' has depth %u. Placing current layer before it.\n", (*iter)->name_.c_str(), (*iter)->depth_);
      #endif

      break;
    }
  }

  if (iter == instances.end())
  {
    #ifdef DEBUG
    printf("SpriteLayer::addLayer(): There is no layer that has greater depth than currently created.\n");
    #endif
  }

  SpriteLayer* layer = new SpriteLayer(depth, name);

  instances.insert(iter, layer);

  #ifdef DEBUG
  printf("SpriteLayer::addLayer(): Layer successfully created.\n");
  #endif

  return 0;
}

SpriteLayer::SpriteLayer(unsigned int depth, std::string name)
:
depth_(depth),
name_(name)
{

}

SpriteLayer* SpriteLayer::getLayer(std::string name)
{
  for (SpriteLayer *layer : instances)
  {
    if (layer->name_ == name)
    {
      return layer;
    }
  }

  printf("SpriteLayer::getLayer(): WARNING! No layer named '%s' found.\n", name.c_str());

  return nullptr;
}

SpriteLayer* SpriteLayer::getLayer(unsigned int depth)
{
  for (SpriteLayer *layer : instances)
  {
    if (layer->depth_ == depth)
    {
      return layer;
    }
  }

  printf("SpriteLayer::getLayer(): WARNING! No layer which depth is '%u' found.\n", depth);

  return nullptr;
}

std::list<Sprite*>::iterator SpriteLayer::addSprite(Sprite *sprite)
{
  sprites_.push_front(sprite);
  return sprites_.begin();
}

void SpriteLayer::removeSprite(std::list<Sprite*>::iterator it)
{
  sprites_.erase(it);
}

void SpriteLayer::debugLogLayers()
{
  for (size_t i = 0; i < instances.size(); ++i)
  {
    #ifdef DEBUG
    printf("SpriteLayer::debugLogLayers(): Layer #%u: depth = %u, name = %s.\n", i, instances[i]->depth_, instances[i]->name_.c_str());
    #endif
  }
}

/*
* SPRITE CLASS
*/

Sprite::Sprite(GameObject *parent, SpriteLayer *layer, Texture *texture)
:
parent_(parent),
positionOffset_({0.0f, 0.0f}),
rotation_(0.0f),
scaling_({1.0f, 1.0f}),

animation_(nullptr)
{
  #ifdef DEBUG
  printf("Sprite::Sprite(): Initializing sprite.\n");
  #endif

  init(texture, layer);

  #ifdef DEBUG
  printf("Sprite::Sprite(): Sprite created.\n");
  #endif
}

void Sprite::init(Texture *texture, SpriteLayer *layer)
{
  setColor({1.0f, 1.0f, 1.0f, 1.0f});

  texture_ = texture;

  if (texture_ != nullptr)
  {
    setSizeFromTexture();
  }
  else
  {
    size_.x = 10.0f;
    size_.y = 10.0f;
  }

  layer_ = layer;

  if (layer_ != nullptr)
  {
    positionInLayer_ = layer_->addSprite(this);
  }
  else
  {
    printf("Sprite::init(): ERROR! Layer is nullptr.\n");
  }
}

Sprite::~Sprite()
{
  layer_->removeSprite(positionInLayer_);
}

void Sprite::setSizeFromTexture()
{
  if (texture_ != nullptr)
  {
    size_.x = (float)texture_ -> width_;
    size_.y = (float)texture_ -> height_;
    
    return;
  }

  printf("Sprite::setSizeFromTexture(): WARNING! No texture assigned to sprite.\n");

  return;
}

void Sprite::setOffset(glm::fvec2 positionOffset)
{
  positionOffset_ = positionOffset;
}

void Sprite::setSize(glm::fvec2 size)
{
  size_ = size;
}

void Sprite::setRotation(float degrees)
{
  rotation_ = degrees;
}

void Sprite::setScaling(glm::fvec2 scaling)
{
  scaling_ = scaling;
}

void Sprite::setColor(glm::fvec4 color)
{
  color_ = color;
}

void Sprite::updateModelMatrix()
{
  modelMatrix_ = glm::mat4(1.0f);

  float xPos = parent_ -> position_.x + positionOffset_.x;
  float yPos = parent_ -> position_.y + positionOffset_.y;

  modelMatrix_ = glm::translate(modelMatrix_, glm::vec3(xPos, yPos, 0.0f));
  modelMatrix_ = glm::rotate(modelMatrix_, glm::radians(rotation_), glm::vec3(0.0, 0.0, 1.0));
  modelMatrix_ = glm::scale(modelMatrix_, glm::vec3(size_.x / 2, size_.y / 2, 1.0));
  modelMatrix_ = glm::scale(modelMatrix_, glm::vec3(scaling_.x, scaling_.y, 1.0));

}