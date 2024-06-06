//
//  Object.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 6.6.2024.
//

#include "Object.hpp"
#include <iostream>

Object::Object(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->indices = indices;
    this->pos = glm::vec3(0.0);
    
    initialize();
}

Object::Object(const Object& obj)
: vertices(obj.vertices), indices(obj.indices), pos(obj.pos) {
    initialize();
}

Object& Object::operator=(const Object& obj) {
    if (this == &obj) return *this;
    
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
    
    vertices = obj.vertices;
    indices = obj.indices;
    pos = obj.pos;
    
    initialize();
    
    return *this;
}

Object::Object(Object&& obj)
: _VAO(obj._VAO), _VBO(obj._VBO), _EBO(obj._EBO),
vertices(std::move(obj.vertices)), indices(std::move(obj.indices)), pos(std::move(obj.pos)) {
    obj._VAO = 0;
    obj._VBO = 0;
    obj._EBO = 0;
}

Object& Object::operator=(Object&& obj) {
    if (this == &obj) return *this;
    std::cout << "move assignment called\n";
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
    
    _VAO = obj._VAO;
    _VBO = obj._VBO;
    _EBO = obj._EBO;
    vertices = std::move(obj.vertices);
    indices = std::move(obj.indices);
    pos = std::move(obj.pos);
    
    obj._VAO = 0;
    obj._VBO = 0;
    obj._EBO = 0;
    
    return *this;
}

void Object::draw() {
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Object::initialize() {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);
    
    glBindVertexArray(_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
