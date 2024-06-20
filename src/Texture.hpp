//
//  Texture.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 5.6.2024.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <glad/glad.h>
#include <string>
#include <stb_image.h>

class Texture {
 public:
  unsigned int ID;
  std::string path;
  std::string type;

  int width, height, nrChannels;

  Texture(std::string path_, std::string type_, GLint internalformat,
          bool flip = false);

  void bind(GLenum unit);
};

#endif /* Texture_hpp */
