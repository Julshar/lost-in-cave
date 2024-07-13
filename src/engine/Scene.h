#pragma once

#include <list>

class Scene
{
public:

  Scene();
  ~Scene();

  /*
  * Override this function in derived class
  * to update derived scene every game tick
  * 
  * Is updated even when engine is in 'pause' state
  */
  virtual void update();

private:

  friend class Engine;

  static std::list <Scene*> instances;
  std::list<Scene*>::iterator instancePosition_;
};