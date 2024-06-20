//
//  ResourcesManager.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 16.6.2024.
//

#include "ResourcesManager.hpp"

#include <iostream>

ResourcesManager::ResourcesManager() {
    meshes = std::vector<Mesh*>();
    textures = std::vector<Texture*>();
    materials = std::vector<Material*>();
    shaders = std::vector<Shader*>();
    _meshesLastIndex = 0;
    _meshesIndicesSize = 0;
    _defaultDiffuse = nullptr;
    _defaultSpecular = nullptr;
}

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

void ResourcesManager::initialize() {
    _defaultDiffuse = loadTexture("texture_diffuse", "./assets/defaultDiffuse.png", GL_RGB);
    _defaultSpecular = loadTexture("texture_specular", "./assets/defaultSpecular.png", GL_RGB);
}

void ResourcesManager::loadMesh(Mesh* mesh) {
    meshes.push_back(mesh);
    mesh->globalStartIndex = _meshesIndicesSize;
    mesh->indicesStartIndex = _meshesLastIndex;
    size_t maxIndexUsedInMesh = 0;
    for (int i = 0; i < mesh->size; i++) {
        if (mesh->localIndices[i] > maxIndexUsedInMesh) {
            maxIndexUsedInMesh = mesh->localIndices[i];
        }
    }
    _meshesLastIndex += maxIndexUsedInMesh + 1;
    _meshesIndicesSize += mesh->size;
    if (_renderer->isInitialized()) {
        // TODO: add to VBO and EBO using subData
    }
};



Texture* ResourcesManager::loadTexture(std::string name, std::string path, GLint internalformat, bool flip) {
    auto isPresented = std::find_if(textures.begin(), textures.end(), [&path](Texture* t_) {return t_->path == path;});
    if (isPresented != textures.end()) {
        return *isPresented;
    }
    Texture* newText = new Texture(path, name, internalformat, flip);
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
    if (std::find_if(textures_.begin(), textures_.end(), [](Texture* t_) {return t_->type == "texture_diffuse";}) == textures_.end()) {
        textures_.push_back(_defaultDiffuse);
    }
    if (std::find_if(textures_.begin(), textures_.end(), [](Texture* t_) {return t_->type == "texture_specular";}) == textures_.end()) {
        textures_.push_back(_defaultSpecular);
    }
    Material* m = new Material(name_, shininess_, textures_, blendColor_);
    return m;
};

Material* ResourcesManager::loadMaterial(std::string name_, float shininess_, glm::vec3 blendColor_) {
    auto isPresented = std::find_if(materials.begin(), materials.end(), [&name_](Material* m_) {return name_ == m_->name;});
    if (isPresented != materials.end()) {
        return *isPresented;
    }
    std::vector<Texture*> text = {_defaultDiffuse, _defaultSpecular};
    Material* m = new Material(name_, shininess_, text, blendColor_);
    return m;
}

Model* ResourcesManager::loadModel(std::string path, std::string name, bool flip) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return nullptr;
    }
    std::vector<Model::MeshMaterialPair> pairs;
    
    std::string directory = "";
    size_t lastSlashPos = path.rfind('/');
    if (lastSlashPos != std::string::npos) {
        // Extract the substring up to and including the last '/'
        directory = path.substr(0, lastSlashPos + 1);
    }
    
    processNode(scene->mRootNode, scene, pairs, directory, name, flip);
    Model* model = new Model(pairs);
    return model;
}

void ResourcesManager::processNode(aiNode *node, const aiScene *scene, std::vector<Model::MeshMaterialPair>& pairs, std::string path, std::string name, bool flip) {
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            // process vertex positions, normals and texture coordinates
            glm::vec3 temp;
            temp.x = mesh->mVertices[i].x;
            temp.y = mesh->mVertices[i].y;
            temp.z = mesh->mVertices[i].z;
            vertex.pos = temp;
            
            if (mesh->HasNormals()) {
                temp.x = mesh->mNormals[i].x;
                temp.y = mesh->mNormals[i].y;
                temp.z = mesh->mNormals[i].z;
                vertex.norm = temp;
            }
            if(mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.text = vec;
                // tangent
                temp.x = mesh->mTangents[i].x;
                temp.y = mesh->mTangents[i].y;
                temp.z = mesh->mTangents[i].z;
                vertex.tangent = temp;
                // bitangent
                temp.x = mesh->mBitangents[i].x;
                temp.y = mesh->mBitangents[i].y;
                temp.z = mesh->mBitangents[i].z;
                vertex.bitangent = temp;
            }
            vertices.push_back(vertex);
        }
        
        for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        Mesh* myMesh = new Mesh(vertices, indices, name + "Mesh");
        loadMesh(myMesh);
        
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture*> textures;
        
        for(unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
            aiString str;
            material->GetTexture(aiTextureType_DIFFUSE, i, &str);
            Texture* text = loadTexture("texture_diffuse", path + str.C_Str(), GL_SRGB_ALPHA, flip);
            textures.push_back(text);
        }
        for(unsigned int i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); i++) {
            aiString str;
            material->GetTexture(aiTextureType_SPECULAR, i, &str);
            Texture* text = loadTexture("texture_specular", path + str.C_Str(), GL_RGB, flip);
            textures.push_back(text);
        }
        for(unsigned int i = 0; i < material->GetTextureCount(aiTextureType_NORMALS); i++) {
            aiString str;
            material->GetTexture(aiTextureType_NORMALS, i, &str);
            Texture* text = loadTexture("texture_normal", path + str.C_Str(), GL_RGB, flip);
            textures.push_back(text);
        }
        
        Material* myMaterial = loadMaterial(name + "Material", 64, textures);
        pairs.push_back({myMesh, myMaterial});
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, pairs, path, name, flip);
    }
}
