#include "Animation.h"

#include <cstdlib>

#undef DEBUG

std::list <Animation*> Animation::instances;

Animation::Animation(std::vector<Texture*> frames)
:
frames_(frames),
animationSpeed_(1),
currentFrame_(0),
frameAdvanceCounter_(0)
{
  instances.push_front(this);
  instancePosition_ = instances.begin();
}

Animation::~Animation()
{
  Animation::instances.erase(instancePosition_);
}

Texture* Animation::getCurrentFrame()
{
  #ifdef DEBUG
  printf("Animation::getCurrentFrame(): Current frame: %i, Total frames: %i, frame advance timer: %i, speed: %i, frame name: %s.\n",
          currentFrame_, frames_.size(), frameAdvanceCounter_, animationSpeed_, frames_[currentFrame_]->name_.c_str());
  #endif

  return frames_[currentFrame_];
}

void Animation::setSpeed(int speed)
{
  animationSpeed_ = speed;
}

void Animation::update()
{
  if (++frameAdvanceCounter_ >= animationSpeed_)
  {
    if (++currentFrame_ >= frames_.size())
    {
      currentFrame_ = 0;
    }
    frameAdvanceCounter_ = 0;
  }
}