#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader *shader)
{
  shader_ = shader;

  initWhiteTexture();
  initGlData();
}

void SpriteRenderer::initWhiteTexture()
{
  unsigned char data[4] = {255, 255, 255, 255};

  glGenTextures(1, &whiteTextureId_);
  glBindTexture(GL_TEXTURE_2D, whiteTextureId_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void SpriteRenderer::initGlData()
{
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

  glGenBuffers(1, &ebo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);  

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1); 

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SpriteRenderer::renderSprites()
{
  for (SpriteLayer *layer : SpriteLayer::instances)
  {
    for (Sprite *sprite : layer->sprites_)
    {
      render(sprite);
    }
  }
}

void SpriteRenderer::render(Sprite *sprite)
{
  sprite->updateModelMatrix();

  shader_ -> setMatrix4f("model", sprite->modelMatrix_);
  shader_ -> setVector4f("color", sprite->color_);
  shader_ -> use();

  if (sprite->animation_ != nullptr)
  {
    (sprite->animation_->getCurrentFrame())->bind(0);
  }
  else if (sprite->texture_ != nullptr)
  {
    sprite->texture_ -> bind(0);
  }
  else
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, whiteTextureId_);
  }

  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void SpriteRenderer::render(UISprite *uiSprite)
{
  if (uiSprite->hidden_ == true)
  {
    return;
  }

  glm::mat4 modelMatrix(1.0f);

  modelMatrix = glm::translate(modelMatrix, glm::vec3(uiSprite->parent_->position_.x + uiSprite->parent_->pivot_.x, uiSprite->parent_->position_.y + uiSprite->parent_->pivot_.y, 0.0f));
  modelMatrix = glm::rotate(modelMatrix, glm::radians(uiSprite->rotation_), glm::vec3(0.0, 0.0, 1.0));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(uiSprite->size_.x / 2, uiSprite->size_.y / 2, 1.0));

  shader_->setMatrix4f("model", modelMatrix);
  shader_->setVector4f("color", uiSprite->color_);

  shader_->use();

  if (uiSprite->animation_ != nullptr)
  {
    (uiSprite->animation_->getCurrentFrame())->bind(0);
  }
  else if (uiSprite->texture_ != nullptr)
  {
    uiSprite->texture_ -> bind(0);
  }
  else
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, whiteTextureId_);
  }

  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}