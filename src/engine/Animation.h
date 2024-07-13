#pragma once

#include "Texture.h"

#include <vector>
#include <list>

class Animation
{
public:
  
  Animation(std::vector<Texture*> frames);
  ~Animation();
  
  Texture* getCurrentFrame();

  void setSpeed(int speed);

private:

  friend class Engine;

  static std::list <Animation*> instances;
  std::list<Animation*>::iterator instancePosition_;

  std::vector<Texture*> frames_;

  int animationSpeed_;
  int currentFrame_;
  int frameAdvanceCounter_;

  void update();

};