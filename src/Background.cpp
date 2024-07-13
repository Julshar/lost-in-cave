#include "Background.h"

Background::Background(glm::fvec2 position, glm::fvec2 scaling)
{
  addSprite(SpriteLayer::getLayer("background"), Texture::getInstance("cave"));
  sprite_->setScaling(scaling);
}