#include "Player.h"
#include "GameParameters.h"
#include "Gem.h"
#include "Hazard.h"

#include "Engine.h"
#include "Physics.h"

#include <vector>
#include <cstdlib>

#undef DEBUG

Player::Player()
:
maxVelocity_(300.0f),
acceleration_(25.0f),
velocityDecay_(5.0f),
lives_(3),
pauseRequest_(false),
isDead_(false)
{
  addCamera();
  setCameraAsActive();

  addSprite(SpriteLayer::getLayer("player"), Texture::getInstance("ship0"));
  sprite_->setSize({GameParameters::PLAYER_SIZE_X, GameParameters::PLAYER_SIZE_Y});

  addPhysics({GameParameters::PLAYER_SIZE_X * 0.90f, GameParameters::PLAYER_SIZE_Y * 0.90f}, 1);
  physics_->collisionGroupId_ = GameParameters::PLAYER_COLLISION_GROUP;
  physics_->groupsToCollideWith_.push_back(GameParameters::WALL_COLLISION_GROUP);
  physics_->updateEdges();

  std::vector <int> keys =
  {
    GLFW_KEY_UP,
    GLFW_KEY_DOWN,
    GLFW_KEY_RIGHT,
    GLFW_KEY_LEFT,

    GLFW_KEY_ESCAPE,

    GLFW_KEY_EQUAL,
    GLFW_KEY_MINUS
  };

  keyInput_ = new KeyInput(keys);
  animFly_ = new Animation
  (
    {
      Texture::getInstance("ship0"),
      Texture::getInstance("ship1"),
      Texture::getInstance("ship2")
    }
  );
  animFly_->setSpeed(10);

  sprite_->animation_ = animFly_;
}

Player::~Player()
{
  delete keyInput_;
  delete animFly_;
}

void Player::update()
{
  #ifdef DEBUG
  std::cout << "Player::update(): Updating player.\n";
  #endif

  if (isDead_ == false)
  {
    processKeyInput();
    processExtraInput();
  }
}

void Player::processKeyInput()
{
  if (keyInput_->isKeyPressed(GLFW_KEY_UP))
  {
    physics_->applyVelocityY(acceleration_);

    if (physics_->getVelocityY() > maxVelocity_)
    {
      physics_->setVelocityY(maxVelocity_);
    }
  }
  else if (keyInput_->isKeyPressed(GLFW_KEY_DOWN))
  {
    physics_->applyVelocityY(acceleration_ * -1.0f);

    if ((physics_->getVelocityY() * -1.0f) > maxVelocity_)
    {
      physics_->setVelocityY(-maxVelocity_);
    }
  }
  else
  {
    if (physics_->getVelocityY() > 0.0f)
    {
      physics_->applyVelocityY(-velocityDecay_);

      if (physics_->getVelocityY() < 0.0f)
      {
        physics_->setVelocityY(0.0f);
      }
    }
    else if (physics_->getVelocityY() < 0.0f)
    {
      physics_->applyVelocityY(velocityDecay_);

      if (physics_->getVelocityY() > 0.0f)
      {
        physics_->setVelocityY(0.0f);
      }
    }
  }

  if (keyInput_->isKeyPressed(GLFW_KEY_RIGHT))
  {
    physics_->applyVelocityX(acceleration_);

    if (physics_->getVelocityX() > maxVelocity_)
    {
      physics_->setVelocityX(maxVelocity_);
    }
  }
  else if (keyInput_->isKeyPressed(GLFW_KEY_LEFT))
  {
    physics_->applyVelocityX(acceleration_ * -1.0f);

    if ((physics_->getVelocityX() * -1.0f) > maxVelocity_)
    {
      physics_->setVelocityX(-maxVelocity_);
    }
  }
  else
  {
    if (physics_->getVelocityX() > 0.0f)
    {
      physics_->applyVelocityX(-velocityDecay_);

      if (physics_->getVelocityX() < 0.0f)
      {
        physics_->setVelocityX(0.0f);
      }
    }
    else if (physics_->getVelocityX() < 0.0f)
    {
      physics_->applyVelocityX(velocityDecay_);

      if (physics_->getVelocityX() > 0.0f)
      {
        physics_->setVelocityX(0.0f);
      }
    }
  }

  if (keyInput_->isKeyPressed(GLFW_KEY_ESCAPE))
  {
    pauseRequest_ = true;
  }
}

void Player::processExtraInput()
{
  float modScale = 0.0f;
  float scaleStep = 0.01f;

  if (keyInput_->isKeyPressed(GLFW_KEY_EQUAL))
  {
    modScale -= scaleStep;
  }
  if (keyInput_->isKeyPressed(GLFW_KEY_MINUS))
  {
    modScale += scaleStep;
  }
  Engine::modifyZoom(modScale);
}