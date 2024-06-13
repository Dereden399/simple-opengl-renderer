//
//  Renderer.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 7.6.2024.
//

#include <algorithm>
#include <iostream>
#include "Renderer.hpp"

Renderer::Renderer() {
    _initialised = false;
    _meshes = std::vector<Mesh*>();
};

Renderer::~Renderer() {
    if (_initialised) {
        glDeleteVertexArrays(1, &_VAO);
        glDeleteBuffers(1, &_VBO);
        glDeleteBuffers(1, &_EBO);
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

void Renderer::drawObjects(Shader* shader, std::vector<Object*>& objects, Camera* camera, std::vector<Light*> lights = std::vector<Light*>()) {
    glBindVertexArray(_VAO);
    shader->use();
    auto projViewMatrix = camera->getProjectionViewMatrix();
    shader->setUniform("projectionView", projViewMatrix);
    shader->setUniform("viewerPos", {camera->pos.x, camera->pos.y, camera->pos.z});
    
    bool hasDirLight = false;
    int pointLightsCount = 0;
    int spotLightsCount = 0;
    
    for (const auto& light : lights) {
        switch (light->getType()) {
            case Light::LightType::DirLight:
            {
                auto dirLight = static_cast<DirectionalLight*>(light);
                if (!hasDirLight) {
                    shader->setUniform("dirLight.lightColor", {dirLight->lightColor.x, dirLight->lightColor.y, dirLight->lightColor.z});
                    shader->setUniform("dirLight.direction", {dirLight->direction.x, dirLight->direction.y, dirLight->direction.z});
                    shader->setUniform("dirLight.intensity", {dirLight->intensity});
                    hasDirLight = true;
                }
                break;
            }
            case Light::LightType::PointLight:
            {
                if (pointLightsCount >= 50) break;
                auto pointLight = static_cast<PointLight*>(light);
                shader->setUniform(("pointLights[" + std::to_string(pointLightsCount) + "].lightColor"), {pointLight->lightColor.x, pointLight->lightColor.y, pointLight->lightColor.z});
                shader->setUniform(("pointLights[" + std::to_string(pointLightsCount) + "].pos"), {pointLight->pos.x, pointLight->pos.y, pointLight->pos.z});
                shader->setUniform(("pointLights[" + std::to_string(pointLightsCount) + "].intensity"), {pointLight->intensity});
                shader->setUniform(("pointLights[" + std::to_string(pointLightsCount) + "].constant"), {pointLight->constant});
                shader->setUniform(("pointLights[" + std::to_string(pointLightsCount) + "].linear"), {pointLight->linear});
                shader->setUniform(("pointLights[" + std::to_string(pointLightsCount) + "].quadratic"), {pointLight->quadratic});
                pointLightsCount++;
                break;
            }
            case Light::LightType::SpotLight:
            {
                if (spotLightsCount >= 50) break;
                auto spotLight = static_cast<SpotLight*>(light);
                shader->setUniform(("spotLights[" + std::to_string(spotLightsCount) + "].lightColor"), {spotLight->lightColor.x, spotLight->lightColor.y, spotLight->lightColor.z});
                shader->setUniform(("spotLights[" + std::to_string(spotLightsCount) + "].direction"), {spotLight->direction.x, spotLight->direction.y, spotLight->direction.z});
                shader->setUniform(("spotLights[" + std::to_string(spotLightsCount) + "].pos"), {spotLight->pos.x, spotLight->pos.y, spotLight->pos.z});
                shader->setUniform(("spotLights[" + std::to_string(spotLightsCount) + "].intensity"), {spotLight->intensity});
                shader->setUniform(("spotLights[" + std::to_string(spotLightsCount) + "].innerCutOff"), {spotLight->innerCutOff});
                shader->setUniform(("spotLights[" + std::to_string(spotLightsCount) + "].outerCutOff"), {spotLight->outerCutOff});
                spotLightsCount++;
                break;
            }
            default:
                break;
        }
    }
    shader->setUniform("hasDirLight", {hasDirLight ? 1.0f : 0.0f});
    shader->setUniform("pointLightsCount", {pointLightsCount});
    shader->setUniform("spotLightsCount", {spotLightsCount});
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
