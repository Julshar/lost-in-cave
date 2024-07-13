#include "Hazard.h"
#include "GameParameters.h"

Hazard::Hazard(glm::fvec2 position, glm::fvec2 size, glm::fvec2 velocity)
{
  addSprite(SpriteLayer::getLayer("hazard"), Texture::getInstance("enemy0"));
  setPosition(position);
  sprite_->setSize(size);

  addPhysics(size * 0.85f, 1);
  physics_->setBounce({1.0f, 1.0f});
  physics_->setVelocity(velocity);
  physics_->collisionGroupId_ = GameParameters::HAZARD_COLLISION_GROUP;
  physics_->groupsToCollideWith_.push_back(GameParameters::WALL_COLLISION_GROUP);
}