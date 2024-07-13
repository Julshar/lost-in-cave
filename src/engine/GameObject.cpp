#include "GameObject.h"

#include <stdio.h>

std::list <GameObject*> GameObject::instances;

GameObject::GameObject()
:
position_({0.0f, 0.0f}),

sprite_(nullptr),
physics_(nullptr),
camera_(nullptr)
{
  instances.push_front(this);
  instancePosition_ = instances.begin();
}

GameObject::~GameObject()
{
  if (sprite_ != nullptr)
  {
    delete sprite_;
  }

  if (physics_ != nullptr)
  {
    delete physics_;
  }

  if (camera_ != nullptr)
  {
    delete camera_;
  }

  GameObject::instances.erase(instancePosition_);
}

void GameObject::updateInstances()
{
  for (GameObject *instance : GameObject::instances)
  {
    instance -> update();
  }
}

/*
* This function can be overriden by user to call it every frame
*/
void GameObject::update()
{

}

void GameObject::addSprite(SpriteLayer *layer, Texture *texture)
{
  sprite_ = new Sprite(this, layer, texture);
}

/*
* WORKAROUND - enum PhysicsType
*
* type:
* 0 = None
* 1 = Dynamic
* 2 = Static
*/
void GameObject::addPhysics(glm::fvec2 size, int type)
{
  physics_ = new Physics(this, size, type);
}

void GameObject::addCamera()
{
  camera_ = new Camera(this);
}

void GameObject::setCameraAsActive()
{
  if (camera_ != nullptr)
  {
    camera_ -> setAsActiveCamera();
  }
  else
  {
    printf("GameObject::setCameraAsActive(): WARNING! This object has no camera assigned.\n");
  }
}

void GameObject::setPosition(glm::fvec2 position)
{
  position_ = position;
}

void GameObject::modifyPosition(glm::fvec2 positionMod)
{
  position_ += positionMod;
}

bool GameObject::isColliding(GameObject *obj)
{
  if (this->physics_ == nullptr)
  {
    printf("GameObject::isColliding(): WARNING! This object has no physics.\n");
  }

  if (obj->physics_ == nullptr)
  {
    printf("GameObject::isColliding(): WARNING! Opposing object has no physics.\n");
  }

  return this->physics_->isColliding(obj->physics_);
}