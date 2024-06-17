//
//  ResourcesManager.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 16.6.2024.
//

#ifndef ResourcesManager_hpp
#define ResourcesManager_hpp

#include "Objects/Mesh.hpp"
#include "Objects/Material.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

class ResourcesManager {
public:
    std::vector<Mesh*> meshes;
    std::vector<Texture*> textures;
    std::vector<Shader*> shaders;
    std::vector<Material*> materials;
    
    ResourcesManager() {
        meshes = std::vector<Mesh*>();
        textures = std::vector<Texture*>();
        materials = std::vector<Material*>();
        shaders = std::vector<Shader*>();
        _meshesLastIndex = 0;
    };
    ~ResourcesManager();
    
    void setRenderer(Renderer* renderer) {_renderer = renderer;};
    void loadMesh(Mesh* mesh);
    Texture* loadTexture(std::string name, std::string path, GLint internalformat, GLenum format, bool flip = false);
    Shader* loadShader(const char* vertexPath, const char* fragmentPath);
    Material* loadMaterial(std::string name_, float shininess_, std::vector<Texture*>& textures_, glm::vec3 blendColor_ = glm::vec3(1.0f));
private:
    Renderer* _renderer;
    unsigned int _meshesLastIndex;
};

#endif /* ResourcesManager_hpp */
