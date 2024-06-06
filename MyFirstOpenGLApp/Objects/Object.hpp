//
//  Object.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 6.6.2024.
//

#ifndef Object_hpp
#define Object_hpp

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Vertex.hpp"

class Object {
protected:
    unsigned int _VAO, _VBO, _EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
public:
    glm::vec3 pos;
    Object(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    Object(const Object& obj);
    Object& operator=(const Object& obj);
    Object(Object&& obj);
    Object& operator=(Object&& obj);
    ~Object() {
        glDeleteVertexArrays(1, &_VAO);
        glDeleteBuffers(1, &_VBO);
        glDeleteBuffers(1, &_EBO);
    };
    
    void draw();
private:
    void initialize();
};

#endif /* Object_hpp */
