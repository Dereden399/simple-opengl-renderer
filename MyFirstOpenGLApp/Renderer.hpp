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
#include "Camera.hpp"
#include "Lights.hpp"

class Renderer {
    unsigned int _VAO, _VBO, _EBO;
    bool _initialised;
    std::vector<Mesh*> _meshes;
public:
    Renderer();
    ~Renderer();
    // we don't want to copy our renderer (yet)
    Renderer(const Renderer& r) = delete;
    Renderer& operator=(const Renderer& r) = delete;
    
    Renderer(Renderer&& r);
    Renderer& operator=(Renderer&& r);
    
    void initialise();
    void addStaticMesh(Mesh* mesh);
    void drawObjects(Shader* shader, std::vector<Object*>& objects, Camera* camera, DirectionalLight* dirLights, std::vector<PointLight*> pointLights);
};

#endif /* Renderer_hpp */
