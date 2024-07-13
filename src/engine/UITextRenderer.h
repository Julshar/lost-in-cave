#pragma once

#include "Texture.h"
#include "Shader.h"
#include "UIText.h"

#include <glm/glm.hpp>

#include <map>

class UITextRenderer
{
public:

  Shader *shader_;

  UITextRenderer(Shader *shader, int windowWidth, int windowHeight);

  void renderText(UIText *text);
  void updateProjectionMatrix(int windowWidth, int windowHeight);

private:

  glm::mat4 UIprojectionMatrix_;

  unsigned int vao_;
  unsigned int vbo_;
};