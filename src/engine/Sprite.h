#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <string>

#include "Texture.h"
#include "Shader.h"
#include "GameObject.h"
#include "Animation.h"

class GameObject;
class Sprite;

class SpriteLayer
{
public:

  static int addLayer(unsigned int depth, std::string name);
  static SpriteLayer* getLayer(std::string name);
  static SpriteLayer* getLayer(unsigned int depth);
  static void debugLogLayers();

private:

  friend class Sprite;
  friend class SpriteRenderer;
  
  static std::vector<SpriteLayer*> instances;

  unsigned int depth_;
  std::string name_;

  std::list<Sprite*> sprites_;

  SpriteLayer(unsigned int depth, std::string name);

  std::list<Sprite*>::iterator addSprite(Sprite *sprite);
  void removeSprite(std::list<Sprite*>::iterator it);

};

class Sprite
{
public:

GameObject *parent_;

/*
* Position is relative to parent game object
*/
glm::fvec2 positionOffset_;
glm::fvec2 size_;
glm::fvec2 scaling_;
// in degrees
float rotation_;

glm::fvec4 color_;
Texture *texture_;
Animation *animation_;

SpriteLayer *layer_;

Sprite(GameObject *parent, SpriteLayer *layer, Texture *texture = nullptr);
~Sprite();

void setSizeFromTexture();
void setOffset(glm::fvec2 positionOffset);
void setSize(glm::fvec2 size);
void setRotation(float degrees);
void setScaling(glm::fvec2 scaling);
void setColor(glm::fvec4 color);

private:

  friend class SpriteRenderer;

  std::list<Sprite*>::iterator positionInLayer_;

  glm::mat4 modelMatrix_;

  void init(Texture *texture, SpriteLayer *layer);

  void updateModelMatrix();
};