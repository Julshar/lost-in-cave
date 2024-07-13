#pragma once

#include "GameObject.h"

class Gem : public GameObject
{
public:

  Gem(glm::fvec2 position, glm::fvec2 size);

  void update();
  
private:

  float scaleModifier_;
  bool scalingDown_;

  void updateGemRotation();
};