#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#undef DEBUG

std::vector<Texture *> Texture::instances;

Texture::Texture(const char *path, std::string name)
:
name_(name)
{
  int ret = init(path);

  if (ret == 1)
  {
    printf("Texture::Texture(): WARNING! Texture initialization failed. This object should be discarded.\n");
  }
}

Texture::Texture(std::string name)
:
name_(name)
{

}

void Texture::loadTexture(const char *path, std::string name)
{
  Texture *tex = new Texture(name);

  if (tex -> init(path) == 1)
  {
    #ifdef DEBUG
    printf("Texture::loadTexture(): Texture initialization failed. Discarding object.\n");
    #endif

    delete tex;
  }
}

void Texture::unloadAllTextures()
{
  Texture *tex;

  while (Texture::instances.empty() == false)
  {
    tex = Texture::instances.back();
    Texture::instances.pop_back();
    tex -> deinitialize();
  }

  #ifdef DEBUG
  printf("Texture::unloadAllTextures(): All textures unloaded.\n");
  #endif
}

Texture *Texture::getInstance(std::string name)
{
  for (Texture *texture : Texture::instances)
  {
    if (texture -> name_ == name)
    {
      return texture;
    }
  }

  printf("Texture::getInstance(): WARNING! No texture named %s found.\n", name.c_str());

  return nullptr;
}

int Texture::init(const char *path)
{
  GLenum format;

  glGenTextures(1, &textureId_);  
  glBindTexture(GL_TEXTURE_2D, textureId_);
  
  stbi_set_flip_vertically_on_load(true);

  #ifdef DEBUG
  printf("Texture::init(): Going to load texture from: %s \n", path);
  #endif

  unsigned char *data = stbi_load(path, &width_, &height_, &channelCount_, 0);

  if (!data)
  {
    printf("Texture::init(): WARNING! Failed to load texture.\n");

    return 1;
  }

  #ifdef DEBUG
  printf("Texture::init(): Loaded image: width = %d, height = %d, channel count = %d.\n",
         width_, height_, channelCount_);
  #endif

  switch (channelCount_)
  {
    case 1:
    {
      format = GL_LUMINANCE;
      break;
    }
    case 2:
    {
      format = GL_LUMINANCE_ALPHA;
      break;
    }
    case 3:
    {
      format = GL_RGB;
      break;
    }
    case 4:
    {
      format = GL_RGBA;
      break;
    }
  }

  // This has to be set if the produced data is tightly packed
  // and stb produces tightly packed data
  // Remember it is set globally so it has to be done just once
  // or set everytime and set back to default which is 4
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
  glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  Texture::instances.push_back(this);

  #ifdef DEBUG
  printf("Texture::init(): Texture successfully created.\n");
  #endif

  stbi_image_free(data);

  return 0;
}

void Texture::deinitialize()
{
  /*
  * TODO:
  * delete this texture data from openGL
  */
}

void Texture::bind(int textureUnit)
{
  /*
  * Texture units exist to make possible using multiple textures in shader program simultaniously
  * After activating texture unit the next bind texture call will bind a texture to activated unit
  * There should be at least 16 texture units
  * example: 7th texture unit can be accessed this way: GL_TEXTURE0 + 6
  */
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_2D, textureId_);
}