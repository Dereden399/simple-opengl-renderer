//
//  Shader.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 4.6.2024.
//

#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <initializer_list>
#include <glm/glm.hpp>



class Shader
{
public:
    // the program ID
    unsigned int ID;
    
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    // use/activate the shader
    void use();
    // utility uniform functions
    void setUniform(const std::string &name, std::initializer_list<bool> values) const;
    void setUniform(const std::string &name, std::initializer_list<int> values) const;
    void setUniform(const std::string &name, std::initializer_list<float> values) const;
    
    void setUniform(const std::string &name, glm::mat4& values) const;
    
};
#endif /* Shader_hpp */
