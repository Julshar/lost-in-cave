#pragma once

#include "Sprite.h"
#include "Physics.h"
#include "Camera.h"

#include <glm/glm.hpp>

class SpriteLayer;
class Sprite;
class Physics;
class Camera;

class GameObject
{
public:

  GameObject();
  ~GameObject();

  glm::fvec2 position_;

  /*
  * Override this function in derived class
  * to update derived object every game tick
  */
  virtual void update();

  void addSprite(SpriteLayer *layer, Texture *texture = nullptr);
  void addPhysics(glm::fvec2 size, int type);
  void addCamera();

  void setCameraAsActive();

  void setPosition(glm::fvec2 position);
  void modifyPosition(glm::fvec2 positionMod);

  bool isColliding(GameObject *obj);

  Sprite *sprite_;
  Physics *physics_;
  Camera *camera_;

  static void updateInstances();
  
private:

  static std::list <GameObject*> instances;
  std::list<GameObject*>::iterator instancePosition_;

};