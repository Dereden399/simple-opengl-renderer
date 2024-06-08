//
//  Renderer.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 7.6.2024.
//

#include <algorithm>
#include <iostream>
#include "Renderer.hpp"

Renderer::Renderer(Shader& shader_) : shader(shader_) {
    _initialised = false;
    _meshes = std::vector<Mesh*>();
};

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
};

void Renderer::initialise() {
    std::vector<Vertex> finalVertices;
    std::vector<unsigned int> finalIndices;
    unsigned int startIndex = 0;
    for (auto mesh : _meshes) {
        mesh->globalStartIndex = startIndex;
        finalVertices.insert(finalVertices.end(), mesh->vertices.begin(), mesh->vertices.end());
        unsigned int maxIndexUsedInMesh = 0;
        for (auto idx : mesh->localIndices) {
            if (idx > maxIndexUsedInMesh) {
                maxIndexUsedInMesh = idx;
            }
            finalIndices.push_back(idx+startIndex);
        }
        startIndex += maxIndexUsedInMesh + 1;
    }
    
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);
    
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, finalVertices.size()*sizeof(Vertex), finalVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, finalIndices.size()*sizeof(unsigned int), finalIndices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    _initialised = true;
};

void Renderer::addStaticMesh(Mesh *mesh) {
    auto wasAdded = std::find_if(_meshes.begin(), _meshes.end(), [&mesh](Mesh* m_) {
        return m_ == mesh;
    });
    if (wasAdded != _meshes.end()) {
        // is already in _meshes
        return;
    }
    _meshes.push_back(mesh);
};

void Renderer::drawObjects(std::vector<Object> &objects) {
    glBindVertexArray(_VAO);
    for (auto& obj : objects) {
        auto model = obj.getModelMatrix();
        shader.setUniform("model", model);
        shader.setUniform("aColor", {obj.color.x, obj.color.y, obj.color.z});
        shader.setUniform("textureMix", {obj.textureMix});
        if (obj.texture != NULL) {
            obj.texture->bind();
        }
        glDrawElements(GL_TRIANGLES, obj.mesh->size, GL_UNSIGNED_INT, (void*)(size_t)(obj.mesh->globalStartIndex));
    }
};
