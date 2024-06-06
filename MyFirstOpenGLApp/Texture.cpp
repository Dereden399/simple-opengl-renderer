//
//  Texture.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 5.6.2024.
//

#include "Texture.hpp"
#include <iostream>

Texture::Texture(std::string path, std::string name, GLint internalformat, GLenum format, bool flip, GLint sWrapMode, GLint tWrapMode, GLint magMode, GLint minMode, GLenum _unit) {
    unit = _unit;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magMode);
    stbi_set_flip_vertically_on_load(flip);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Texture::bind() {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}
