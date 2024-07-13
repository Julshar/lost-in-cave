#pragma once

#include "Shader.h"
#include "Sprite.h"
#include "UIButton.h"
#include "UISprite.h"

class SpriteRenderer
{
public:

  Shader *shader_;

  SpriteRenderer(Shader *shader);

  void renderSprites();

  void render(UISprite *uiSprite);
  
private:

  float vertices_[32] = 
  {   // positions       // texture
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // top right
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, // top left
  };

  unsigned int indices_[6] = 
  {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
  };

  unsigned int whiteTextureId_;

  unsigned int vao_;
  unsigned int vbo_;
  unsigned int ebo_;

  void initWhiteTexture();
  void initGlData();

  void render(Sprite *sprite);
};