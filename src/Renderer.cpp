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
  _initialized = false;
  _hdrShader = nullptr;
};

Renderer::~Renderer() {
  if (_initialized) {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
    glDeleteBuffers(1, &_LightsUBO);
    glDeleteFramebuffers(1, &_hdrFBO);
    glDeleteRenderbuffers(1, &_rboDepth);
  }
};

void Renderer::initialize(std::vector<Mesh*> meshes, Mesh* renderMesh) {
  if (_initialized) return;

  std::vector<Vertex> finalVertices;
  std::vector<unsigned int> finalIndices;

  finalVertices.insert(finalVertices.end(), renderMesh->vertices.begin(),
                       renderMesh->vertices.end());
  finalIndices.insert(finalIndices.end(), renderMesh->localIndices.begin(),
                      renderMesh->localIndices.end());

  for (const auto& mesh : meshes) {
    finalVertices.insert(finalVertices.end(), mesh->vertices.begin(),
                         mesh->vertices.end());
    for (int i = 0; i < mesh->size; i++) {
      finalIndices.push_back(mesh->localIndices[i] + mesh->indicesStartIndex);
    }
  }

  glGenVertexArrays(1, &_VAO);
  glGenBuffers(1, &_VBO);
  glGenBuffers(1, &_EBO);
  glGenBuffers(1, &_LightsUBO);
  glGenFramebuffers(1, &_hdrFBO);

  glGenTextures(1, &_colorBuffer);
  glBindTexture(GL_TEXTURE_2D, _colorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800 * 2, 600 * 2, 0, GL_RGBA,
               GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glGenRenderbuffers(1, &_rboDepth);
  glBindRenderbuffer(GL_RENDERBUFFER, _rboDepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800 * 2, 600 * 2);

  glBindVertexArray(_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, finalVertices.size() * sizeof(Vertex),
               finalVertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               finalIndices.size() * sizeof(unsigned int), finalIndices.data(),
               GL_STATIC_DRAW);

  const size_t lightsUBOSize = 5664;
  glBindBuffer(GL_UNIFORM_BUFFER, _LightsUBO);
  glBufferData(GL_UNIFORM_BUFFER, lightsUBOSize, NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, _LightsUBO);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, pos));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, norm));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, text));
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, tangent));
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, bitangent));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         _colorBuffer, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, _rboDepth);
  // glEnable(GL_DEPTH_TEST);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Framebuffer not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3),
                  glm::value_ptr(camera->position));
  bool hasDirLight = false;
  int pointLightsCount = 0;
  int spotLightsCount = 0;

  size_t dirLightOffset = 5632;
  size_t spotLightsOffset = 2432;
  size_t pointLightsOffset = 32;

  for (const auto& light : lights) {
    auto type = light->getType();
    switch (type) {
      case Light::LightType::DirLight: {
        auto dirLight = static_cast<DirectionalLight*>(light);
        auto d = (dirLight->info);
        if (!hasDirLight) {
          glBufferSubData(GL_UNIFORM_BUFFER, dirLightOffset, sizeof(d), &d);
          hasDirLight = true;
        }
        break;
      }
      case Light::LightType::PointLight: {
        if (pointLightsCount >= 50) break;
        auto pointLight = static_cast<PointLight*>(light);
        auto d = (pointLight->info);
        d.pos =
            glm::vec3(pointLight->toParentTransform * glm::vec4(d.pos, 1.0f));
        glBufferSubData(GL_UNIFORM_BUFFER,
                        pointLightsOffset + pointLightsCount * 48, sizeof(d),
                        &d);
        pointLightsCount++;
        break;
      }
      case Light::LightType::SpotLight: {
        if (spotLightsCount >= 50) break;
        auto spotLight = static_cast<SpotLight*>(light);
        auto d = (spotLight->info);
        d.pos =
            glm::vec3(spotLight->toParentTransform * glm::vec4(d.pos, 1.0f));
        d.direction = glm::vec3(spotLight->toParentTransform *
                                glm::vec4(d.direction, 0.0f));
        glBufferSubData(GL_UNIFORM_BUFFER,
                        spotLightsOffset + spotLightsCount * 64, sizeof(d), &d);
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

void Renderer::traverseNode(
    Node* node, std::unordered_map<Shader*, std::vector<Model*>>& models,
    std::vector<Light*>& lights) {
  node->toParentTransform = node->parentNode != nullptr
                                ? node->parentNode->toParentTransform *
                                      node->parentNode->getModelMatrix()
                                : glm::mat4(1.0f);
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
    traverseNode(n, models, lights);
  }
};

void Renderer::drawNode(Node* node, Camera* camera) {
  std::unordered_map<Shader*, std::vector<Model*>> models;
  std::vector<Light*> lights;
  traverseNode(node, models, lights);
  glBindVertexArray(_VAO);
  setLightsUBO(lights, camera);
  glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (auto& [key, value] : models) {
    drawModels(key, value, camera);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void Renderer::drawModels(Shader* shader, std::vector<Model*>& models,
                          Camera* camera) {
  shader->use();
  auto projViewMatrix = camera->getProjectionViewMatrix();
  shader->setUniform("projectionView", projViewMatrix);

  for (auto& model : models) {
    auto modelMatrix = model->toParentTransform * model->getModelMatrix();
    shader->setUniform("model", modelMatrix);
    auto normalsModel = glm::transpose(glm::inverse(modelMatrix));
    shader->setUniform("normalsModel", normalsModel);
    shader->setUniform("textureScale",
                       {model->tileTextures ? model->scale.x : 1.0f});
    bool useNormalMap = false;
    bool useEmissionMap = false;

    for (const auto& pair : model->meshes) {
      auto& material = pair.material;
      auto& mesh = pair.mesh;

      shader->setUniform("material.shininess", {material->shininess});
      shader->setUniform("material.emissionStrength",
                         {material->emissionStrength});
      int diffNum = 0;
      int specNum = 0;
      int normNum = 0;
      int emitNum = 0;
      for (int i = 0; i < material->textures.size(); i++) {
        material->textures[i]->bind(GL_TEXTURE0 + i);
        std::string type = material->textures[i]->type;
        std::string number;
        if (type == "texture_diffuse")
          number = std::to_string(diffNum++);
        else if (type == "texture_specular")
          number = std::to_string(specNum++);
        else if (type == "texture_normal") {
          number = std::to_string(normNum++);
          useNormalMap = true;
        } else if (type == "texture_emission") {
          number = std::to_string(emitNum++);
          useEmissionMap = true;
        }
        shader->setUniform(("material." + type + number).c_str(), {i});
      }
      shader->setUniform("material.useNormalMap", {useNormalMap});
      shader->setUniform("material.useEmissionMap", {useEmissionMap});
      shader->setUniform("material.blendColor",
                         {material->blendColor.x, material->blendColor.y,
                          material->blendColor.z, 1.0f});
      // std::cout << glGetError() << std::endl;
      glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT,
                     (void*)(mesh->globalStartIndex * sizeof(unsigned int)));
      // std::cout << glGetError() << std::endl;
    }
  }
};

void Renderer::applyHdr(float exposure) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _hdrShader->use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _colorBuffer);
  _hdrShader->setUniform("hdrBuffer", {0});
  _hdrShader->setUniform("exposure", {exposure});
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}
