#include "TextFont.h"

#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdio.h>

#undef DEBUG

std::vector<TextFont *> TextFont::instances;

TextFont* TextFont::getInstance(std::string name)
{
	for (TextFont *font : TextFont::instances)
  {
    if (font -> name_ == name)
    {
      return font;
    }
  }

  printf("TextFont::getInstance(): WARNING! No font named %s found.\n", name.c_str());

  return nullptr;
}

void TextFont::loadFont(std::string filePath, unsigned int fontSize, std::string name)
{
  TextFont *font = new TextFont(name);

  if (font -> init(filePath, fontSize) != 0)
  {
    printf("TextFont::loadFont(): WARNING! Font initialization failed. Discarding object.\n");
    delete font;
  }

  #ifdef DEBUG
  printf("TextFont::loadFont(): Font %s successfully loaded.\n", name.c_str());
  #endif
}

TextFont::TextFont(std::string name)
:
name_(name)
{

}

int TextFont::init(std::string filePath, unsigned int fontSize)
{
  FT_Library ft;
  FT_Face face;
  int error;

  error = FT_Init_FreeType(&ft);

  if (error)
  {
    printf("TextFont::init(): ERROR! Could not init FreeType Library.\n");

    return 1;
  }

  error = FT_New_Face(ft, filePath.c_str(), 0, &face);

  if (error != 0)
  {
    printf("TextFont::init(): ERROR! Failed to load font.\n");

    if (error == FT_Err_Unknown_File_Format)
    {
      printf("TextFont::init(): ERROR! Font file could be opened and read, but font format is unsupported.\n");
    }
    else if (error)
    {
      printf("TextFont::init(): ERROR! Font file could not be opened, read or it is broken.\n");
    }

    return 1;
  }

  FT_Set_Pixel_Sizes(face, 0, fontSize);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
  
  for (unsigned char c = 0; c < 128; ++c)
  {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      printf("TextFont::init(): ERROR! Failed to load glyph.\n");

      return 1;
    }
    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /*
    * In case of Character.advance
    * using bitshift by 6 to get value in pixels
    * since freeType provides this value in 1/64 of a pixel
    */
    Character character = 
    {
      texture,
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      (face->glyph->advance.x >> 6)
    };
  
    characters_.insert(std::pair<char, Character>(c, character));
  }

  instances.push_back(this);

  glBindTexture(GL_TEXTURE_2D, 0);
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  return 0;
}

TextFont::~TextFont()
{
  /*
  * TODO: removing text fonts
  *
  * Can implement removing text fonts or not allow it
  * since it is probably a bad idea to load font and later
  * unload it during game time.
  */
}

unsigned int TextFont::getTextHeight()
{
  return characters_['H'].size.y;
}

unsigned int TextFont::getTextWidth(std::string *text)
{
  unsigned int width = 0;

  for (char c : *text)
  {
    width += characters_[c].advance;
  }

  return width;
}