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
#include <unordered_map>

Renderer::Renderer() {
    _initialized = false;
};

Renderer::~Renderer() {
    if (_initialized) {
        glDeleteVertexArrays(1, &_VAO);
        glDeleteBuffers(1, &_VBO);
        glDeleteBuffers(1, &_EBO);
        glDeleteBuffers(1, &_LightsUBO);
    }
};

void Renderer::initialize(std::vector<Mesh*> meshes) {
    if (_initialized) return;
    
    std::vector<Vertex> finalVertices;
    std::vector<unsigned int> finalIndices;
    for (const auto& mesh : meshes) {
        finalVertices.insert(finalVertices.end(), mesh->vertices.begin(), mesh->vertices.end());
        for (int i = 0; i < mesh->size; i++) {
            finalIndices.push_back(mesh->localIndices[i] + mesh->globalStartIndex);
        }
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
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, text));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    _initialized = true;
};

Renderer::Renderer(Renderer&& r)
: _VAO(r._VAO), _VBO(r._VBO), _EBO(r._EBO), _initialized(r._initialized) {
    // Leave the source object in a valid state
    r._VAO = 0;
    r._VBO = 0;
    r._EBO = 0;
    r._initialized = false;
}

Renderer& Renderer::operator=(Renderer&& r) {
    if (this != &r) {
        // Clean up existing resources if initialised
        if (_initialized) {
            glDeleteVertexArrays(1, &_VAO);
            glDeleteBuffers(1, &_VBO);
            glDeleteBuffers(1, &_EBO);
        }

        // Move data from the source object
        _VAO = r._VAO;
        _VBO = r._VBO;
        _EBO = r._EBO;
        _initialized = r._initialized;

        // Leave the source object in a valid state
        r._VAO = 0;
        r._VBO = 0;
        r._EBO = 0;
        r._initialized = false;
    }
    return *this;
}

void Renderer::setLightsUBO(std::vector<Light*>& lights, Camera* camera) {
    glBindBuffer(GL_UNIFORM_BUFFER, _LightsUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(camera->position));
    bool hasDirLight = false;
    int pointLightsCount = 0;
    int spotLightsCount = 0;
    
    size_t dirLightOffset = 5632;
    size_t spotLightsOffset = 2432;
    size_t pointLightsOffset = 32;
    
    for (const auto& light : lights) {
        auto type = light->getType();
        switch (type) {
            case Light::LightType::DirLight:
            {
                auto dirLight = static_cast<DirectionalLight*>(light);
                auto d = (dirLight->info);
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
                auto d = (pointLight->info);
                glBufferSubData(GL_UNIFORM_BUFFER, pointLightsOffset + pointLightsCount*48, sizeof(d), &d);
                pointLightsCount++;
                break;
            }
            case Light::LightType::SpotLight:
            {
                if (spotLightsCount >= 50) break;
                auto spotLight = static_cast<SpotLight*>(light);
                auto d = (spotLight->info);
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

void Renderer::drawNode(Node* node, Camera* camera) {
    std::unordered_map<Shader*, std::vector<Model*>> models;
    std::vector<Light*> lights;
    for (const auto& n : node->children) {
        switch (n->getNodeType()) {
            case Node::NodeType::Model: {
                auto model = static_cast<Model*>(n);
                models[model->shader].push_back(model);
                break;
            }
            case Node::NodeType::Light: {
                auto light = static_cast<Light*>(n);
                lights.push_back(light);
                break;
            }
            default:
                break;
        }
    }
    glBindVertexArray(_VAO);
    setLightsUBO(lights, camera);
    for (auto& [key, value] : models) {
        drawModels(key, value, camera);
    }
};

void Renderer::drawModels(Shader* shader, std::vector<Model*>& models, Camera* camera) {
    shader->use();
    auto projViewMatrix = camera->getProjectionViewMatrix();
    shader->setUniform("projectionView", projViewMatrix);
    
    for (auto& model : models) {
        auto modelMatrix = model->getModelMatrix();
        shader->setUniform("model", modelMatrix);
        auto normalsModel = glm::transpose(glm::inverse(modelMatrix));
        shader->setUniform("normalsModel", normalsModel);
        
        for (const auto& pair : model->meshes) {
            auto& material = pair.material;
            auto& mesh = pair.mesh;
            
            shader->setUniform("material.shininess", {material->shininess});
            int diffNum = 0;
            int specNum = 0;
            for (int i = 0; i < material->textures.size(); i++) {
                material->textures[i]->bind(GL_TEXTURE0 + i);
                std::string type = material->textures[i]->type;
                std::string number;
                if(type == "texture_diffuse")
                    number = std::to_string(diffNum++);
                else if(type == "texture_specular")
                    number = std::to_string(specNum++);
                shader->setUniform(("material." + type + number).c_str(), {i});
            }
            shader->setUniform("material.blendColor", {material->blendColor.x,material->blendColor.y,material->blendColor.z, 1.0f});
            //std::cout << glGetError() << std::endl;
            glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT, (void*)(size_t)(mesh->globalStartIndex));
            //std::cout << glGetError() << std::endl;
        }
    }
};
