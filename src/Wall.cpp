#include "Wall.h"
#include "GameParameters.h"

Wall::Wall(glm::fvec2 position, glm::fvec2 size)
{
  addSprite(SpriteLayer::getLayer("wall"), Texture::getInstance("wall"));
  setPosition(position);
  sprite_->setSize(size);
  addPhysics(size * 0.95f, 2);
  physics_->collisionGroupId_ = GameParameters::WALL_COLLISION_GROUP;
}

Wall::~Wall()
{

}

void Wall::update()
{

}