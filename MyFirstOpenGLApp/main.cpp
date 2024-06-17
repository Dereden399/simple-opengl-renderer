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
#include "Objects/Model.hpp"
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
    
    Shader* uberShader = program.resourcesManager.loadShader("./Shaders/vertexShader.glsl", "./Shaders/fragmentShader.glsl");
    
    Texture* boxTexture = program.resourcesManager.loadTexture("texture_diffuse", "Assets/container.jpg", GL_RGB, GL_RGB);
    Texture* containerDiffuse = program.resourcesManager.loadTexture("texture_diffuse", "Assets/container2.png", GL_RGB, GL_RGBA);
    Texture* containerSpecularMap = program.resourcesManager.loadTexture("texture_specular", "Assets/container2_specular.png", GL_RGB, GL_RGBA);
    
    CubeMesh* cubeMesh = new CubeMesh();
    program.resourcesManager.loadMesh(cubeMesh);
    
    std::vector<Texture*> boxMaterialTextures = {boxTexture};
    Material* boxMaterial = program.resourcesManager.loadMaterial("boxCrateMaterial", 64, boxMaterialTextures);
    
    std::vector<Texture*> containerMaterialTextures = {containerDiffuse, containerSpecularMap};
    Material* containerMaterial = program.resourcesManager.loadMaterial("containerMaterial", 64, containerMaterialTextures);
    
    program.renderer.initialize(program.resourcesManager.meshes);
    
    Model box = Model(cubeMesh, boxMaterial);
    box.shader = uberShader;

    Model container = Model(cubeMesh, containerMaterial);
    container.shader = uberShader;
    
    Model floor = Model(cubeMesh, containerMaterial);
    floor.shader = uberShader;
    
    box.position = glm::vec3(2.0f, 0.0f, 0.0f);
    floor.position = glm::vec3(0.0, -1.0f, 0.0f);
    container.position = glm::vec3(-2.0f, 0.0f, 0.0f);
    
    floor.scale = glm::vec3(10.0f, 0.1f, 10.0f);
    
    Node scene = Node();
    scene.children.push_back(&box);
    scene.children.push_back(&floor);
    scene.children.push_back(&container);
    
    DirectionalLight light = DirectionalLight(glm::vec3(1.0f,1.0f,1.0f), glm::normalize(glm::vec3(0.0f, 2.0f, 1.0f)), 0.4f, 0.1f);
    
    scene.children.push_back(&light);
    
    PointLight pointLight = PointLight(glm::vec3(0.8f,1.0f,1.0f), glm::vec3(0.0f,-0.5f,-1.0f), 1.0f, 0.2f, 1.0f, 0.2f, 0.15f);
    PointLight pointLight2 = PointLight(glm::vec3(0.0f,1.0f,1.0f), glm::vec3(0.0f,-0.5f,-1.0f), 1.0f, 0.2f, 1.0f, 0.2f, 0.15f);
    PointLight pointLight3 = PointLight(glm::vec3(1.0f,0.0f,1.0f), glm::vec3(3.0f,-0.5f,-1.0f), 1.0f, 0.2f, 1.0f, 0.2f, 0.15f);
    
    scene.children.push_back(&pointLight);
    scene.children.push_back(&pointLight2);
    scene.children.push_back(&pointLight3);
    
    SpotLight spotlight = SpotLight(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(0.0f,0.0f,3.0f), glm::normalize(glm::vec3(0.0f, 2.0f, 1.0f)), 1.0f, 0.1f, 0.97f, 0.96f);
    
    //scene.children.push_back(&spotlight);
    
    Camera camera = Camera(glm::vec3(0.0f,0.0f,-3.0f), glm::vec3(0.0f,1.0f,0.0f));
    program.selectedCamera = &camera;
    
    program.mainNode = &scene;
    
    // render loop
    while(!program.shouldWindowClose()) {
        auto [time, delta] = program.startRenderLoop();
        box.rotate({delta*30, delta*30, 0});
        box.position.y = sin(time)/4 + 1;
        
        pointLight.info.pos.x = sin(time*2)*4;
        pointLight.info.pos.z = cos(time*2)*3;
        
        spotlight.info.pos = camera.position;
        spotlight.info.direction = camera.getBasisForward();
        
        program.endRenderLoop();
    }
    glfwTerminate();
    return 0;
}
