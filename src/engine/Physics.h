#pragma once

#include "GameObject.h"

#include <glm/glm.hpp>

#include <list>
#include <vector>

class GameObject;

class Physics
{
public:

  enum class PhysicsType
  {
    None,
    Dynamic,
    Static
  };

  Physics(GameObject *parent, glm::fvec2 size, int type = 0);
  ~Physics();

  GameObject *parent_;

  Physics::PhysicsType type_;

  glm::fvec2 size_;

  int collisionGroupId_;
  std::vector<int> groupsToCollideWith_;

  float getVelocityX();
  float getVelocityY();
  glm::fvec2 getVelocity();

  void applyVelocity(glm::fvec2 velocity);
  void applyVelocityX(float x);
  void applyVelocityY(float y);
  void setVelocity(glm::fvec2 velocity);
  void setVelocityX(float x);
  void setVelocityY(float y);

  void setBounce(glm::fvec2 bounce);

  void updateEdges();
  bool isColliding(Physics *obj);
  bool isColliding(glm::fvec4 edges);
  bool isCollidingWithGroup(int group);
  std::vector<GameObject*> collisionWithGroup(int group);

  static void updateInstances();

private:

  struct Stats
  {
    glm::fvec2 velocity;
    glm::fvec2 position;
  };

  struct Edges
  {
    float left;
    float right;
    float bottom;
    float top;
  };

  Stats current_;
  Stats future_;

  Edges edges_;

  /*
  * Determines what happens with objects velocity after collision
  */
  glm::fvec2 bounce_;

  static std::list <Physics*> instances;
  std::list<Physics*>::iterator instancePosition_;

  void calculateFutureStats();
  void updateCurrentStats();
  void calculateCurrentEdges();
  Edges calculateEdges(glm::fvec2 *position);

  static float framesPerSecond;
  static float minUnit;

  void handleCollisionWithStatic(Physics *obj);
  PhysicsType getPhysicsTypeFromInt(int type);

  void update();
};