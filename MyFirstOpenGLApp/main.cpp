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
    
    Shader* uberShader = program.resourcesManager.loadShader("./Shaders/vertexShader.glsl", "./Shaders/BlinnPhongFragment.glsl");
    
    Texture* containerDiffuse = program.resourcesManager.loadTexture("texture_diffuse", "Assets/container2.png", GL_SRGB_ALPHA, GL_RGBA);
    Texture* containerSpecularMap = program.resourcesManager.loadTexture("texture_specular", "Assets/container2_specular.png", GL_RGB, GL_RGBA);
    
    Texture* brickwallDiffuse = program.resourcesManager.loadTexture("texture_diffuse", "Assets/brickwall.jpg", GL_SRGB, GL_RGB);
    Texture* brickwallNormal = program.resourcesManager.loadTexture("texture_normal", "Assets/brickwall_normal.jpg", GL_RGB, GL_RGB);
    
    CubeMesh* cubeMesh = new CubeMesh();
    program.resourcesManager.loadMesh(cubeMesh);
    
    
    std::vector<Texture*> containerMaterialTextures = {containerDiffuse, containerSpecularMap};
    Material* containerMaterial = program.resourcesManager.loadMaterial("containerMaterial", 16, containerMaterialTextures);
    
    std::vector<Texture*> brickWallTextures = {brickwallDiffuse, brickwallNormal};
    std::vector<Texture*> brickWallWithoutNormalTextures = {brickwallDiffuse};
    Material* brickwallMaterial = program.resourcesManager.loadMaterial("brickwallMaterial", 64, brickWallTextures);
    Material* brickwallWithoutNormalsMaterial = program.resourcesManager.loadMaterial("brickwallWithoutNormalMaterial", 64, brickWallWithoutNormalTextures);
    
    program.renderer.initialize(program.resourcesManager.meshes);
    
    Model box = Model(cubeMesh, brickwallMaterial);
    box.shader = uberShader;

    Model container = Model(cubeMesh, brickwallWithoutNormalsMaterial);
    container.shader = uberShader;
    
    Model floor = Model(cubeMesh, brickwallMaterial);
    floor.shader = uberShader;
    
    box.position = glm::vec3(2.0f, 0.0f, 0.0f);
    floor.position = glm::vec3(0.0, -1.0f, 0.0f);
    container.position = glm::vec3(-2.0f, 0.0f, 0.0f);
    
    floor.scale = glm::vec3(10.0f, 0.1f, 10.0f);
    
    Node scene = Node();
    scene.addChild(&box);
    scene.addChild(&floor);
    scene.addChild(&container);
    //scene.addChild(&container);
    
    DirectionalLight light = DirectionalLight(glm::vec3(1.0f,1.0f,1.0f), glm::normalize(glm::vec3(1.0f, -2.0f, 0.0f)), 0.2f, 0.5f);
    
    //scene.addChild(&light);
    
    PointLight pointLight = PointLight(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(0.0f,3.0f,3.0f), 2.0f, 0.1f, 1.0f, 0.14f, 0.07f);
    PointLight pointLight2 = PointLight(glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), 2.0f, 0.1f, 1.0f, 0.14f, 0.07f);
    PointLight pointLight3 = PointLight(glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f,0.0f,0.0f), 2.0f, 0.1f, 1.0f, 0.14f, 0.07f);
    
    scene.addChild(&pointLight);
    scene.addChild(&pointLight2);
    scene.addChild(&pointLight3);
    
    SpotLight spotlight = SpotLight(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(1.0f,0.0f,0.0f), glm::normalize(glm::vec3(2.0f, -1.0f, 0.0f)), 1.0f, 0.1f, 0.96f, 0.94f);
    
    //scene.addChild(&spotlight);
    
    Camera camera = Camera(glm::vec3(0.0f,0.0f,-3.0f), glm::vec3(0.0f,1.0f,0.0f));
    program.selectedCamera = &camera;
    
    program.mainNode = &scene;
    
    // render loop
    while(!program.shouldWindowClose()) {
        auto [time, delta] = program.startRenderLoop();
        
        //box.position.y = sin(time)/4 + 1;
        
        
          pointLight.info.pos.x = sin(time)*3;
          pointLight.info.pos.z = cos(time)*3;
        
        pointLight2.info.pos.x = sin(time*2+1)*3;
        pointLight2.info.pos.y = cos(time*2+1)*3;
        
        pointLight3.info.pos.z = sin(time*2+2)*3;
        pointLight3.info.pos.y = cos(time*2+2)*3;
        
        spotlight.info.pos = camera.position;
        spotlight.info.direction = camera.getBasisForward();
        
        program.endRenderLoop();
    }
    glfwTerminate();
    return 0;
}
