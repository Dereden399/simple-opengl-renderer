//
//  main.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 25.5.2024.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <iostream>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"
#include "Objects/Object.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "Program.hpp"
#include "Objects/Material.hpp"
#include "Lights.hpp"


int main()
{
    Program program = Program();
    if (program.initialise() == -1) {
        return -1;
    }
    
    Texture boxTexture = Texture("Assets/container.jpg", "boxTexture", GL_RGB, GL_RGB, false, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    
    CubeMesh cubeMesh = CubeMesh();
    
    Shader uberShader = Shader("./Shaders/vertexShader.glsl", "./Shaders/fragmentShader.glsl");
    Material boxCrateMaterial = Material("boxCrateMaterial", glm::vec3(1.0f), glm::vec3(1.0f), 64);
    boxCrateMaterial.diffuseMap = &boxTexture;
    
    Material untexturedMaterial = Material("untexturedMaterial", glm::vec3(1.0f,0.0f,1.0f), glm::vec3(0.0f,1.0f,1.0f), 64);
    
    program.renderer.addStaticMesh(&cubeMesh);
    program.renderer.initialise();
    
    Object box = Object(&cubeMesh);
    box.material = &boxCrateMaterial;
    box.shader = &uberShader;
    
    Object cube = Object(&cubeMesh);
    cube.material = &untexturedMaterial;
    cube.shader = &uberShader;
    
    box.pos = glm::vec3(2.0f, 0.0f, 0.0f);
    cube.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    
    program.objects.push_back(&box);
    program.objects.push_back(&cube);
    
    DirectionalLight light = DirectionalLight();
    light.direction = glm::normalize(glm::vec3(0.0f, -0.5f, -1.0f));
    light.intensity = 1.0f;
    light.lightColor = glm::vec3(1.0f,1.0f,1.0f);
    
    program.dirLight = &light;
    
    Camera camera = Camera(glm::vec3(0.0f,0.0f,3.0f), glm::vec3(0.0f,1.0f,0.0f));
    camera.pointAt(cube.pos);
    program.selectedCamera = &camera;
    
    program.renderer.initialise();
    
    // render loop
    while(!program.shouldWindowClose()) {
        auto [time, delta] = program.startRenderLoop();
        
        box.rotation.x += delta*30;
        box.rotation.y += delta*30;
        box.pos.y = sin(time)/4;
        light.lightColor.x = sin(time)/2+0.5;
        
        program.render();
        
        program.endRenderLoop();
    }
    glfwTerminate();
    return 0;
}
