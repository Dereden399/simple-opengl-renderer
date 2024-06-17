//
//  ResourcesManager.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 16.6.2024.
//

#include "ResourcesManager.hpp"

ResourcesManager::~ResourcesManager() {
    for (const auto& m : meshes) {
        delete m;
    }
    for (const auto& m : materials) {
        delete m;
    }
    for (const auto& m : textures) {
        delete m;
    }
    for (const auto& m : shaders) {
        delete m;
    }
}

void ResourcesManager::loadMesh(Mesh* mesh) {
    meshes.push_back(mesh);
    mesh->globalStartIndex = _meshesLastIndex;
    unsigned int maxIndexUsedInMesh = 0;
    for (int i = 0; i < mesh->size; i++) {
        if (mesh->localIndices[i] > maxIndexUsedInMesh) {
            maxIndexUsedInMesh = mesh->localIndices[i];
        }
    }
    _meshesLastIndex += maxIndexUsedInMesh + 1;
    if (_renderer->isInitialized()) {
        // TODO: add to VBO and EBO using subData
    }
};

Texture* ResourcesManager::loadTexture(std::string name, std::string path, GLint internalformat, GLenum format, bool flip) {
    auto isPresented = std::find_if(textures.begin(), textures.end(), [&path](Texture* t_) {return t_->path == path;});
    if (isPresented != textures.end()) {
        return *isPresented;
    }
    Texture* newText = new Texture(path, name, internalformat, format, flip);
    textures.push_back(newText);
    return newText;
};

Shader* ResourcesManager::loadShader(const char *vertexPath, const char *fragmentPath) {
    Shader* newSh = new Shader(vertexPath, fragmentPath);
    return newSh;
}

Material* ResourcesManager::loadMaterial(std::string name_, float shininess_, std::vector<Texture*>& textures_, glm::vec3 blendColor_) {
    auto isPresented = std::find_if(materials.begin(), materials.end(), [&name_](Material* m_) {return name_ == m_->name;});
    if (isPresented != materials.end()) {
        return *isPresented;
    }
    Material* m = new Material(name_, shininess_, textures_, blendColor_);
    return m;
};
