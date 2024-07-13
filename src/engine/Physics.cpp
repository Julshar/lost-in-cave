#include "Physics.h"

#include <cstdlib>

#undef DEBUG

std::list <Physics*> Physics::instances;
float Physics::framesPerSecond = 60.0f;
float Physics::minUnit = 0.001f;

Physics::Physics(GameObject *parent, glm::fvec2 size, int type)
:
parent_(parent),

collisionGroupId_(0),
size_(size),
bounce_({0.0f, 0.0f})
{
  type_ = getPhysicsTypeFromInt(type);

  current_ = 
  {
    {0.0f, 0.0f},
    {parent_->position_.x, parent_->position_.y}
  };

  if (type_ == PhysicsType::Static)
  {
    this->calculateCurrentEdges();
  }

  instances.push_front(this);
  instancePosition_ = instances.begin();
}

/*
* WORKAROUND - enum PhysicsType
*
* This function is a workaround effect of enum Physics::PhysicsType
* not being possible to access from other classes (speciffically GameObject)
* despite being in public section of Physics.
*
* This can probably be solved somehow...
*/
Physics::PhysicsType Physics::getPhysicsTypeFromInt(int type)
{
  switch (type)
  {
    case 0:
    {
      return PhysicsType::None;
    }
    case 1:
    {
      return PhysicsType::Dynamic;
    }
    case 2:
    {
      return PhysicsType::Static;
    }
    default:
    {
      printf("Physics::getPhysicsTypeFromInt(): WARNING! Undefined physics type. Returning PhysicsType::None.\n");
      return PhysicsType::None;
    }
  }
}

Physics::~Physics()
{
  Physics::instances.erase(instancePosition_);
}

float Physics::getVelocityX()
{
  return current_.velocity.x;
}

float Physics::getVelocityY()
{
  return current_.velocity.y;
}

glm::fvec2 Physics::getVelocity()
{
  return current_.velocity;
}

void Physics::applyVelocity(glm::fvec2 velocity)
{
  current_.velocity += velocity;
}

void Physics::applyVelocityX(float x)
{
  current_.velocity.x += x;
}

void Physics::applyVelocityY(float y)
{
  current_.velocity.y += y;
}

void Physics::setVelocity(glm::fvec2 velocity)
{
  current_.velocity = velocity;
}

void Physics::setVelocityX(float x)
{
  current_.velocity.x = x;
}

void Physics::setVelocityY(float y)
{
  current_.velocity.y = y;
}

/*
* Bounce value has to be greater than 0 on both x and y
*/
void Physics::setBounce(glm::fvec2 bounce)
{
  if (bounce.x < 0 || bounce.y < 0)
  {
    printf("Physics::setBounce(): WARNING! Bounce values have to be greater than 0. Ignoring set request.\n");
  }
  else
  {
    bounce_ = bounce;
  }
}

void Physics::updateEdges()
{
  this->calculateCurrentEdges();
}

bool Physics::isColliding(Physics *obj)
{
  if (this->edges_.right > obj->edges_.left && this->edges_.left < obj->edges_.right && this->edges_.top > obj->edges_.bottom && this->edges_.bottom < obj->edges_.top)
  {
    return true;
  }

  return false;
}

/*
* edges - {left, right, bottom, top}
*/
bool Physics::isColliding(glm::fvec4 edges)
{
  if (this->edges_.right > edges.x && this->edges_.left < edges.y && this->edges_.top > edges.z && this->edges_.bottom < edges.w)
  {
    return true;
  }

  return false;
}

/*
* Returns true if 'this' collides with any
* object of group 'group'
*/
bool Physics::isCollidingWithGroup(int group)
{
  for (Physics *instance : instances)
  {
    if (instance->collisionGroupId_ == group)
    {
      if (isColliding(instance))
      {
        return true;
      }
    }
  }

  return false;
}

/*
* Returns vector of game objects
* which are colliding with 'this' object
* and which physics group is equal to passed 'group'
*/
std::vector<GameObject*> Physics::collisionWithGroup(int group)
{
  std::vector<GameObject*> collidingObjects;

  for (Physics *instance : instances)
  {
    if (instance->collisionGroupId_ == group)
    {
      if (isColliding(instance))
      {
        collidingObjects.push_back(instance->parent_);
      }
    }
  }

  return collidingObjects;
}

void Physics::updateInstances()
{
  for (Physics *instance : Physics::instances)
  {
    if (instance->type_ == PhysicsType::Dynamic)
    {
      instance -> calculateFutureStats();
      instance -> calculateCurrentEdges();
    }
  }

  for (Physics *instance : Physics::instances)
  {
    if (instance->type_ == PhysicsType::Dynamic)
    {
      instance -> update();
    }
  }

  for (Physics *instance : Physics::instances)
  {
    if (instance->type_ == PhysicsType::Dynamic)
    {
      instance -> updateCurrentStats();
    }
  }
}

void Physics::calculateFutureStats()
{
  future_.velocity = current_.velocity;

  future_.position.x = current_.position.x + current_.velocity.x / framesPerSecond;
  future_.position.y = current_.position.y + current_.velocity.y / framesPerSecond;
}

void Physics::updateCurrentStats()
{
  parent_->position_.x = future_.position.x;
  parent_->position_.y = future_.position.y;
  
  current_.position = future_.position;
  current_.velocity = future_.velocity;
}

/*
* Updates the 'edges_' field based on current position
*/
void Physics::calculateCurrentEdges()
{
  edges_ =
  {
    current_.position.x - (size_.x / 2),
    current_.position.x + (size_.x / 2),
    current_.position.y - (size_.y / 2),
    current_.position.y + (size_.y / 2)
  };
}

inline Physics::Edges Physics::calculateEdges(glm::fvec2 *position)
{
  return
  {
    position->x - (size_.x / 2),
    position->x + (size_.x / 2),
    position->y - (size_.y / 2),
    position->y + (size_.y / 2)
  };
}

void Physics::update()
{

  #ifdef DEBUG
  printf("Physics::update(): Updating instance.\n");
  printf("Physics::update(): Current position (%f, %f).\nPhysics::update(): Future position (%f, %f).\nPhysics::update(): Current velocity: (%f, %f).\nPhysics::update(): Future velocity: (%f, %f).\n",
            current_.positionX_,
            current_.positionY_,
            future_.positionX_,
            future_.positionY_,
            current_.velocityX_,
            current_.velocityY_,
            future_.velocityX_,
            future_.velocityY_
            );
  #endif

  /*
  * Check if future position of object collides with other object
  */
  for (int groupId : groupsToCollideWith_)
  {
    #ifdef DEBUG
    printf("Physics::update(): Looking for collision group %i.\n", groupId);
    #endif

    for (Physics *instance : instances)
    {
      if (instance->collisionGroupId_ == groupId)
      {
        if (instance->type_ == PhysicsType::Static)
        {
          this->handleCollisionWithStatic(instance);
        }
      }
    }
  }

  #ifdef DEBUG
  printf("Physics::update(): Finished updating instance.\n");
  #endif
}

void Physics::handleCollisionWithStatic(Physics *obj)
{
  if (obj->type_ != PhysicsType::Static)
  {
    printf("Physics::handleCollisionWithStatic(): WARNING! Opposing object is not of type 'Static'.\n");
    return;
  }

  Edges futureEdges;
  futureEdges = this->calculateEdges(&future_.position);

  /*
  *  TODO: Improvement
  *
  * Probably can limit the amout of times that edges of dynamic objects are calculated
  * by storing them into object state
  * 
  * also at some point it should be possible to copy edges instead of calculate them
  * (future edges copied to current edges)
  * 
  */

  #ifdef DEBUG
  printf("Physics::handleCollisionWithStatic(): Calculated (left, right, bottom, top) coordinates of objects.\n");
  printf("Physics::handleCollisionWithStatic(): This (%f, %f, %f, %f).\n", futureEdges.left, futureEdges.right, futureEdges.bottom, futureEdges.top);
  printf("Physics::handleCollisionWithStatic(): Opposing (%f, %f, %f, %f).\n", obj->edges_.left, obj->edges_.right, obj->edges_.bottom, obj->edges_.top);
  #endif

  if (futureEdges.right > obj->edges_.left && futureEdges.left < obj->edges_.right && futureEdges.top > obj->edges_.bottom && futureEdges.bottom < obj->edges_.top)
  {
    #ifdef DEBUG
    printf("Physics::handleCollisionWithStatic(): Objects are colliding.\n");
    #endif

    /*
    * Each dynamic object has its 'edges_' calculated before updating.
    * So at this point 'this->edges_' has values of edges for 'current_.position'
    */

    #ifdef DEBUG
    printf("Physics::handleCollisionWithStatic(): Calculated (left, right, bottom, top) coordinates of object.\n");
    printf("Physics::handleCollisionWithStatic(): This (previous) (%f, %f, %f, %f).\n", this->edges_.left, this->edges_.right, this->edges_.bottom, this->edges_.top);
    #endif

    /*
    * ASSUMPTION: second object "obj" is static (not moving)
    */

    bool wereCollidingOnX = false;
    bool wereCollidingOnY = false;

    /*
    * Determine on which axes the 2 objects were colliding pervious frame
    */

    if (this->edges_.right > obj->edges_.left && this->edges_.left < obj->edges_.right)
    {
      wereCollidingOnX = true;
    }

    if (this->edges_.top > obj->edges_.bottom && this->edges_.bottom < obj->edges_.top)
    {
      wereCollidingOnY = true;
    }

    if (wereCollidingOnX && !wereCollidingOnY)
    {
      #ifdef DEBUG
      printf("Physics::handleCollisionWithStatic(): Previously objects were colliding only on X axis.\n");
      #endif

      if (current_.velocity.y > 0)
      {
        /*
        * 'this' object was approaching from bottom side
        * Setting its position under static object and modifying Y speed
        */

        future_.position.y = obj->edges_.bottom - minUnit - (this->size_.y / 2);
        future_.velocity.y *= (bounce_.y * -1.0f);
      }
      else if (current_.velocity.y < 0)
      {
        /*
        * 'this' object was approaching from top side
        * Setting its position above static object and modifying Y speed
        */

        future_.position.y = obj->edges_.top + minUnit + (this->size_.y / 2);
        future_.velocity.y *= (bounce_.y * -1.0f);
      }
      else
      {
        printf("Physics::handleCollisionWithStatic(): WARNING! Object was not moving vertically but approached vertically. This should not happen.\n");
      }
    }
    else if (!wereCollidingOnX && wereCollidingOnY)
    {
      #ifdef DEBUG
      printf("Physics::handleCollisionWithStatic(): Previously objects were colliding only on Y axis.\n");
      #endif

      if (current_.velocity.x > 0)
      {
        /*
        * 'this' object was approaching from left side
        * Setting its position left of static object and modifying X speed
        */

        future_.position.x = obj->edges_.left - minUnit - (this->size_.x / 2);
        future_.velocity.x *= (bounce_.x * -1.0f);
      }
      else if (current_.velocity.x < 0)
      {
        /*
        * 'this' object was approaching from right side
        * Setting its position right of static object and modifying X speed
        */

        future_.position.x = obj->edges_.right + minUnit + (this->size_.x / 2);
        future_.velocity.x *= (bounce_.x * -1.0f);
      }
      else
      {
        printf("Physics::handleCollisionWithStatic(): WARNING! Object was not moving horizontally but approached horizontally. This should not happen.\n");
      }
    }
    else if (!wereCollidingOnX && !wereCollidingOnY)
    {
      #ifdef DEBUG
      printf("Physics::handleCollisionWithStatic(): Previously objects were colliding on none of the axes.\n");
      #endif

      /* 
      * solution 1) 
      * check on which axis do objects overlap less
      * then make them stick to each other on that axis
      * and zero / revert velocity on that axis
      * 
      * solution 2)
      * check which velocity is greater (absolute values)
      * then make moving object stick to static object
      * on the axis with smaller velocity
      * and zero / revert smaller velocity
      * 
      * here we use 2)
      */

      if (abs(current_.velocity.x) > abs(current_.velocity.y))
      {
        if (current_.velocity.y > 0)
        {
          /*
          * 'this' object was approaching from bottom side
          * Setting its position under static object and modifying Y speed
          */

          future_.position.y = obj->edges_.bottom - minUnit - (this->size_.y / 2);
          future_.velocity.y *= (bounce_.y * -1.0f);
        }
        else if (current_.velocity.y < 0)
        {
          /*
          * 'this' object was approaching from top side
          * Setting its position above static object and modifying Y speed
          */

          future_.position.y = obj->edges_.top + minUnit + (this->size_.y / 2);
          future_.velocity.y *= (bounce_.y * -1.0f);
        }
        else
        {
          printf("Physics::handleCollisionWithStatic(): WARNING! Object was not moving vertically but approached vertically. This should not happen.\n");
        }
      }
      else
      {
        if (current_.velocity.x > 0)
        {
          /*
          * 'this' object was approaching from left side
          * Setting its position left of static object and modifying X speed
          */

          future_.position.x = obj->edges_.left - minUnit - (this->size_.x / 2);
          future_.velocity.x *= (bounce_.x * -1.0f);
        }
        else if (current_.velocity.x < 0)
        {
          /*
          * 'this' object was approaching from right side
          * Setting its position right of static object and modifying X speed
          */

          future_.position.x = obj->edges_.right + minUnit + (this->size_.x / 2);
          future_.velocity.x *= (bounce_.x * -1.0f);
        }
        else
        {
          printf("Physics::handleCollisionWithStatic(): WARNING! Object was not moving horizontally but approached horizontally. This should not happen.\n");
        }
      } 
    }
    else if (wereCollidingOnX && wereCollidingOnY)
    {
      printf("Physics::handleCollisionWithStatic(): WARNING! Previously objects were colliding on both axes.\n");
    }
  }
}