#include "Gem.h"
#include "GameParameters.h"
#include "Random.h"

Gem::Gem(glm::fvec2 position, glm::fvec2 size)
:
scaleModifier_(0.02f),
scalingDown_(true)
{
  std::string textureName;
  float scalingX = 0.0f;
  unsigned int seed = Random::genValue(0, 2);
  switch (seed)
  {
    case 0:
    {
      textureName = "gem0";
      scalingX = 0.0f;
      break;
    }
    case 1:
    {
      textureName = "gem1";
      scalingX = 0.5f;
      break;
    }
    case 2:
    {
      textureName = "gem2";
      scalingX = -0.5f;
      break;
    }
  }

  addSprite(SpriteLayer::getLayer("gem"), Texture::getInstance(textureName));
  setPosition(position);
  sprite_->setSize(size);
  sprite_->scaling_.x = scalingX;

  addPhysics(size * 0.75f, 2);
  physics_->collisionGroupId_ = GameParameters::GEM_COLLISION_GROUP;
}

void Gem::update()
{
  updateGemRotation();
}

void Gem::updateGemRotation()
{
  if (scalingDown_)
  {
    sprite_->scaling_.x -= scaleModifier_;

    if (sprite_->scaling_.x <= -1.0f)
    {
      sprite_->scaling_.x = -1.0f;
      scalingDown_ = false;
    }
  }
  else
  {
    sprite_->scaling_.x += scaleModifier_;

    if (sprite_->scaling_.x >= 1.0f)
    {
      sprite_->scaling_.x = 1.0f;
      scalingDown_ = true;
    }
  }
}