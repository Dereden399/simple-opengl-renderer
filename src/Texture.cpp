//
//  Texture.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 5.6.2024.
//

#include "Texture.hpp"
#include <iostream>

Texture::Texture(std::string path_, std::string type_, GLint internalformat,
                 bool flip) {
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  stbi_set_flip_vertically_on_load(flip);
  unsigned char *data =
      stbi_load(path_.c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    GLenum format;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 3)
      format = GL_RGB;
    else if (nrChannels == 4)
      format = GL_RGBA;
    else
      format = GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  path = path_;
  type = type_;
}

void Texture::bind(GLenum unit) {
  glActiveTexture(unit);
  glBindTexture(GL_TEXTURE_2D, ID);
}
