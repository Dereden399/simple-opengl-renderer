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

#include "Shader.hpp"
#include "Objects/Mesh.hpp"
#include "Objects/Object.hpp"

class Renderer {
    unsigned int _VAO, _VBO, _EBO;
    bool _initialised;
    std::vector<Mesh*> _meshes;
public:
    Shader& shader;
    
    Renderer(Shader& shader_);
    ~Renderer();
    // we don't want to copy/move our renderer (yet)
    Renderer(const Renderer& r) = delete;
    Renderer(const Renderer&& r) = delete;
    Renderer& operator=(const Renderer& r) = delete;
    Renderer& operator=(const Renderer&& r) = delete;
    
    void initialise();
    void addStaticMesh(Mesh* mesh);
    void drawObjects(std::vector<Object>& objects);
};

#endif /* Renderer_hpp */
