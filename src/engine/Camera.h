#pragma once

#include "GameObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GameObject;

class Camera
{
public:

  Camera(GameObject *parent);
  ~Camera();

  GameObject *parent_;

  glm::fvec2 positionOffset_;

  void setPositionOffset(float x, float y);
  void modifyPositionOffset(float x, float y);

  glm::mat4 getViewMatrix();

  void setAsActiveCamera();
  static Camera* getActiveCamera();

private:

  static constexpr float CAMERA_Z_POS = 0.0f;

  static std::list <Camera*> instances;
  static Camera *activeCamera;

  std::list<Camera*>::iterator instancePosition_;

  void updateViewMatrix();

  glm::mat4 viewMatrix_;
  
};