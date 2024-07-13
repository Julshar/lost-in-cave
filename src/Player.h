#pragma once

#include "GameObject.h"
#include "KeyInput.h"

class Player : public GameObject
{
public:

  int lives_;
  bool pauseRequest_;
  bool isDead_;

  Player();
  ~Player();

  void update();

private:

  KeyInput *keyInput_;
  Animation *animFly_;

  float maxVelocity_;
  float acceleration_;
  float velocityDecay_;

  void processKeyInput();
  void processExtraInput();
};