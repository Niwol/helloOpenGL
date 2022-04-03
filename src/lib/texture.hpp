#pragma once

#include "opengl_stuff.h"
#include <string>

class Texture {
  friend class Renderer;

public:
  Texture(std::string name = "texture");
  ~Texture();

  /**
   * Sets the name of the texture which is used for the fragment shader
   */
  inline void setName(std::string name) { m_name = name; }

  /**
   * Gets the name of the texture which is used for the fragment shader
   */
  inline std::string getName() { return m_name; }

  /**
   * Loads the image located at the `path` and saves it in the Texture
   */
  bool load(std::string path);

private:
  std::string m_name;
  GLuint m_textureID;
};