#include "Scene.h"

std::list<Scene*> Scene::instances;

Scene::Scene()
{
  instances.push_front(this);
  instancePosition_ = instances.begin();
}

Scene::~Scene()
{
  instances.erase(instancePosition_);
}

void Scene::update()
{

}