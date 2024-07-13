#include "Camera.h"

std::list <Camera*> Camera::instances;
Camera *Camera::activeCamera = nullptr;

Camera::Camera(GameObject *parent)
:
parent_(parent),

positionOffset_({0.0f, 0.0f})
{
  updateViewMatrix();

  instances.push_front(this);
  instancePosition_ = instances.begin();
}

Camera::~Camera()
{
  Camera::instances.erase(instancePosition_);
}

void Camera::setPositionOffset(float x, float y)
{
  positionOffset_.x = x;
  positionOffset_.y = y;
}

void Camera::modifyPositionOffset(float x, float y)
{
  positionOffset_.x += x;
  positionOffset_.y += y;
}

glm::mat4 Camera::getViewMatrix()
{
  updateViewMatrix();

  return viewMatrix_;
}

void Camera::setAsActiveCamera()
{
  activeCamera = this;
}

Camera* Camera::getActiveCamera()
{
  if (activeCamera == nullptr)
  {
    printf("Camera::getActiveCamera(): WARNING! No active camera at this point.\n");
  }

  return activeCamera;
}

void Camera::updateViewMatrix()
{
  viewMatrix_ = glm::mat4(1.0f);

  float xPos = parent_ -> position_.x + positionOffset_.x;
  float yPos = parent_ -> position_.y + positionOffset_.y;

  viewMatrix_ = glm::translate(viewMatrix_, glm::vec3(xPos * -1, yPos * -1, CAMERA_Z_POS));
}