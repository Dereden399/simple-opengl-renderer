//
//  Shader.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 4.6.2024.
//

#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <initializer_list>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << std::strerror(errno) << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
       
    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
      
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
      
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
      
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setUniform(const std::string &name, std::initializer_list<bool> values) const {
    switch (values.size()) {
        case 0:
            std::cout << "ERROR::SHADER::PROGRAM::SET_UNIFORM: zero arguments in function call" << std::endl;
            break;
        case 1:
            glUniform1iv(glGetUniformLocation(ID, name.c_str()), 1, (const int *)values.begin());
            break;
        case 2:
            glUniform2iv(glGetUniformLocation(ID, name.c_str()), 1, (const int *)values.begin());
            break;
        case 3:
            glUniform3iv(glGetUniformLocation(ID, name.c_str()), 1, (const int *)values.begin());
            break;
        default:
            glUniform4iv(glGetUniformLocation(ID, name.c_str()), 1, (const int *)values.begin());
    }
}

void Shader::setUniform(const std::string &name, std::initializer_list<int> values) const {
    switch (values.size()) {
        case 0:
            std::cout << "ERROR::SHADER::PROGRAM::SET_UNIFORM: zero arguments in function call" << std::endl;
            break;
        case 1:
            glUniform1iv(glGetUniformLocation(ID, name.c_str()), 1, values.begin());
            break;
        case 2:
            glUniform2iv(glGetUniformLocation(ID, name.c_str()), 1, values.begin());
            break;
        case 3:
            glUniform3iv(glGetUniformLocation(ID, name.c_str()), 1, values.begin());
            break;
        default:
            glUniform4iv(glGetUniformLocation(ID, name.c_str()), 1, values.begin());
    }
}

void Shader::setUniform(const std::string &name, std::initializer_list<float> values) const {
    switch (values.size()) {
        case 0:
            std::cout << "ERROR::SHADER::PROGRAM::SET_UNIFORM: zero arguments in function call" << std::endl;
            break;
        case 1:
            glUniform1fv(glGetUniformLocation(ID, name.c_str()), 1, values.begin());
            break;
        case 2:
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1,  values.begin());
            break;
        case 3:
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1,  values.begin());
            break;
        default:
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1,  values.begin());
    }
}
