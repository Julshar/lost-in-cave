#pragma once

#include "GameObject.h"

class Wall : public GameObject
{
public:

  Wall(glm::fvec2 position, glm::fvec2 size);
  ~Wall();

  void update();

private:

};