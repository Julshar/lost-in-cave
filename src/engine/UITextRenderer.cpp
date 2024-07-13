#include "UITextRenderer.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

#undef DEBUG

UITextRenderer::UITextRenderer(Shader *shader, int windowWidth, int windowHeight)
{
  shader_ = shader;

  updateProjectionMatrix(windowWidth, windowHeight);

  glGenVertexArrays(1, &this->vao_);
  glBindVertexArray(this->vao_);

  glGenBuffers(1, &this->vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void UITextRenderer::updateProjectionMatrix(int windowWidth, int windowHeight)
{
  #ifdef DEBUG
  printf("UITextRenderer::updateProjectionMatrix(): Updating projection matrix.\n");
  #endif

  UIprojectionMatrix_ = glm::ortho(0.0f, static_cast<float>(windowWidth), static_cast<float>(windowHeight), 0.0f);

  shader_ -> setMatrix4f("projection", UIprojectionMatrix_);
}

void UITextRenderer::renderText(UIText *text)
{
  if (text->hidden_ == true)
  {
    return;
  }

  TextFont *font = text->font_;
  std::string::const_iterator i;
  float xOffset = text->parent_->position_.x + text->parent_->pivot_.x + text->positionOffset_.x;

  if (font == nullptr)
  {
    printf("UITextRenderer::renderText(): WARNING! Font is nullptr.\n");
  }

  shader_->setVector4f("textColor", text -> color_);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(this->vao_);
  shader_->use();

  /*
  * Loop to render each character
  */
  for (i = text->content_.begin(); i != text->content_.end(); i++)
  {
    Character *ch = &font->characters_[*i];

    float xPos = xOffset + ch->bearing.x;
    float yPos = text->parent_->position_.y + text->parent_->pivot_.y + text->positionOffset_.y + (font->characters_['H'].bearing.y - ch->bearing.y);

    float width = ch->size.x;
    float height = ch->size.y;

    float vertices[6][4] = 
    {
      { xPos,         yPos + height, 0.0f, 1.0f }, // top left
      { xPos + width, yPos,          1.0f, 0.0f }, // bot right
      { xPos,         yPos,          0.0f, 0.0f }, // bot left

      { xPos,         yPos + height, 0.0f, 1.0f }, // top left
      { xPos + width, yPos + height, 1.0f, 1.0f }, // top right
      { xPos + width, yPos,          1.0f, 0.0f }  // bot right
    };

    glBindTexture(GL_TEXTURE_2D, ch->textureID);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    /*
    * Advance horizontal position for next glyph to render
    */
    xOffset += (ch->advance);
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}