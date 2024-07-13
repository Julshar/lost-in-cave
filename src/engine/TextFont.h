#pragma once

#include <map>
#include <vector>
#include <string>

#include <glm/glm.hpp>

struct Character 
{
  unsigned int textureID; // ID handle of the glyph texture
  glm::ivec2   size;      // size of glyph
  glm::ivec2   bearing;   // offset from baseline to left/top of glyph
  unsigned int advance;   // horizontal offset to advance to next glyph in pixels
};

class TextFont
{
public:

  std::string name_;

  std::map<char, Character> characters_;

  ~TextFont();

  unsigned int getTextHeight();
  unsigned int getTextWidth(std::string *text);

  static void loadFont(std::string filePath, unsigned int fontSize, std::string name);
  static TextFont* getInstance(std::string name);

private:

  TextFont(std::string name);
  int init(std::string filePath, unsigned int fontSize);

  static std::vector<TextFont *> instances;
};