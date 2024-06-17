//
//  Renderer.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 7.6.2024.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <glad/glad.h>
#include <vector>
#include <unordered_map>

#include "Shader.hpp"
#include "Objects/Mesh.hpp"
#include "Objects/Model.hpp"
#include "Camera.hpp"
#include "Lights.hpp"
#include "Objects/Node.hpp"

class Renderer {
    unsigned int _VAO, _VBO, _EBO, _LightsUBO;
    bool _initialized;
    
    void traverseNode(Node* node, std::unordered_map<Shader*, std::vector<Model*>>& models, std::vector<Light*>& lights);
    
public:
    Renderer();
    ~Renderer();
    // we don't want to copy our renderer (yet)
    Renderer(const Renderer& r) = delete;
    Renderer& operator=(const Renderer& r) = delete;
    
    Renderer(Renderer&& r);
    Renderer& operator=(Renderer&& r);
    
    void initialize(std::vector<Mesh*> meshes);
    bool isInitialized() {return _initialized;};
    void drawModels(Shader* shader, std::vector<Model*>& models, Camera* camera);
    void drawNode(Node* node, Camera* camera);
    void setLightsUBO(std::vector<Light*>& lights, Camera* camera);
};

#endif /* Renderer_hpp */
