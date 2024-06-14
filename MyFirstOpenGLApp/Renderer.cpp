//
//  Renderer.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 7.6.2024.
//

#include <algorithm>
#include <iostream>
#include "Renderer.hpp"
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer() {
    _initialised = false;
    _meshes = std::vector<Mesh*>();
};

Renderer::~Renderer() {
    if (_initialised) {
        glDeleteVertexArrays(1, &_VAO);
        glDeleteBuffers(1, &_VBO);
        glDeleteBuffers(1, &_EBO);
        glDeleteBuffers(1, &_LightsUBO);
    }
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
    glGenBuffers(1, &_LightsUBO);
    
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, finalVertices.size()*sizeof(Vertex), finalVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, finalIndices.size()*sizeof(unsigned int), finalIndices.data(), GL_STATIC_DRAW);
    
    const size_t lightsUBOSize = 5664;
    glBindBuffer(GL_UNIFORM_BUFFER, _LightsUBO);
    glBufferData(GL_UNIFORM_BUFFER, lightsUBOSize, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, _LightsUBO);
    
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
    if (_initialised) {
        std::cout << "ERROR::RENDERER: Can't add static mesh after initialization" << std::endl;
        return;
    }
    auto wasAdded = std::find_if(_meshes.begin(), _meshes.end(), [&mesh](Mesh* m_) {
        return m_ == mesh;
    });
    if (wasAdded != _meshes.end()) {
        // is already in _meshes
        return;
    }
    _meshes.push_back(mesh);
};

void Renderer::setLightsUBO(std::vector<Light*>& lights, Camera* camera) {
    glBindBuffer(GL_UNIFORM_BUFFER, _LightsUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(camera->pos));
    bool hasDirLight = false;
    int pointLightsCount = 0;
    int spotLightsCount = 0;
    
    size_t dirLightOffset = 5632;
    size_t spotLightsOffset = 2432;
    size_t pointLightsOffset = 32;
    
    for (const auto& light : lights) {
        // horrible hack with pointer sizes because derived classes contain a hidden pointer to a vtable at offset 0, thus moving the whole information on sizeof(pointer)
        switch (light->getType()) {
            case Light::LightType::DirLight:
            {
                auto dirLight = static_cast<DirectionalLight*>(light);
                auto d = dirLight->getStruct();
                if (!hasDirLight) {
                    glBufferSubData(GL_UNIFORM_BUFFER, dirLightOffset, sizeof(d), &d);
                    hasDirLight = true;
                }
                break;
            }
            case Light::LightType::PointLight:
            {
                if (pointLightsCount >= 50) break;
                auto pointLight = static_cast<PointLight*>(light);
                auto d = pointLight->getStruct();
                glBufferSubData(GL_UNIFORM_BUFFER, pointLightsOffset + pointLightsCount*48, sizeof(d), &d);
                pointLightsCount++;
                break;
            }
            case Light::LightType::SpotLight:
            {
                if (spotLightsCount >= 50) break;
                auto spotLight = static_cast<SpotLight*>(light);
                auto d = spotLight->getStruct();
                glBufferSubData(GL_UNIFORM_BUFFER, spotLightsOffset + spotLightsCount*64, sizeof(d), &d);
                spotLightsCount++;
                break;
            }
            default:
                break;
        }
    }
    float dirLightTerm = hasDirLight ? 1.0f : 0.0f;
    glBufferSubData(GL_UNIFORM_BUFFER, 12, sizeof(float), &dirLightTerm);
    glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(int), &pointLightsCount);
    glBufferSubData(GL_UNIFORM_BUFFER, 20, sizeof(int), &spotLightsCount);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
};

void Renderer::drawObjects(Shader* shader, std::vector<Object*>& objects, Camera* camera) {
    glBindVertexArray(_VAO);
    shader->use();
    auto projViewMatrix = camera->getProjectionViewMatrix();
    shader->setUniform("projectionView", projViewMatrix);

    for (auto& obj : objects) {
        auto model = obj->getModelMatrix();
        shader->setUniform("model", model);
        auto normalsModel = glm::transpose(glm::inverse(model));
        shader->setUniform("normalsModel", normalsModel);
        const auto& material = obj->material;
        if (material->diffuseMap != nullptr) {
            material->diffuseMap->bind(GL_TEXTURE0);
            shader->setUniform("diffuseMap", {0});
        }
        shader->setUniform("useDiffuseMap", {material->diffuseMap != nullptr});
        if (material->specularMap != nullptr) {
            material->specularMap->bind(GL_TEXTURE1);
            shader->setUniform("specularMap", {1});
        }
        shader->setUniform("useSpecularMap", {material->specularMap != nullptr});
        shader->setUniform("diffuseColor", {material->diffuseColor.x, material->diffuseColor.y, material->diffuseColor.z});
        shader->setUniform("specularColor", {material->specularColor.x, material->specularColor.y, material->specularColor.z});
        shader->setUniform("shininess", {material->shininess});
        
        glDrawElements(GL_TRIANGLES, obj->mesh->size, GL_UNSIGNED_INT, (void*)(size_t)(obj->mesh->globalStartIndex));
    }
};
