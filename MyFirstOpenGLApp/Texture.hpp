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
    const char* name[256];
    
    int width, height, nrChannels;
    
    GLenum unit;
    
    Texture(std::string path, std::string name, GLint internalformat, GLenum format, bool flip, GLint sWrapMode, GLint tWrapMode, GLint magMode, GLint minMode, GLenum unit);
    
    void bind();
};

#endif /* Texture_hpp */
