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
    
    Object floor = Object(&cubeMesh);
    floor.material = &boxCrateMaterial;
    floor.shader = &uberShader;
    
    Object cube = Object(&cubeMesh);
    cube.material = &untexturedMaterial;
    cube.shader = &uberShader;
    
    box.pos = glm::vec3(2.0f, 0.0f, 0.0f);
    floor.pos = glm::vec3(0.0, -1.0f, 0.0f);
    cube.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    
    floor.scale = glm::vec3(10.0f, 0.1f, 10.0f);
    
    program.objects.push_back(&box);
    program.objects.push_back(&floor);
    program.objects.push_back(&cube);
    
    DirectionalLight light = DirectionalLight();
    light.direction = glm::normalize(glm::vec3(0.0f, 2.0f, 1.0f));
    light.intensity = 1.0f;
    light.lightColor = glm::vec3(1.0f,1.0f,1.0f);
    
    //program.lights.push_back(&light);
    
    PointLight pointLight = PointLight();
    pointLight.lightColor = glm::vec3(1.0f,1.0f,1.0f);
    pointLight.pos = glm::vec3(0.0f,-0.5f,-1.0f);
    pointLight.constant = 1.0f;
    pointLight.linear = 0.22f;
    pointLight.quadratic = 0.2f;
    pointLight.intensity = 1.0f;
    
    PointLight pointLight2 = PointLight();
    pointLight2.lightColor = glm::vec3(0.0f,1.0f,1.0f);
    pointLight2.pos = glm::vec3(1.0f,-0.5f,1.0f);
    pointLight2.constant = 1.0f;
    pointLight2.linear = 0.22f;
    pointLight2.quadratic = 0.2f;
    pointLight2.intensity = 1.0f;
    
    PointLight pointLight3 = PointLight();
    pointLight3.lightColor = glm::vec3(1.0f,0.0f,0.0f);
    pointLight3.pos = glm::vec3(3.0f,-0.5f,0.0f);
    pointLight3.constant = 1.0f;
    pointLight3.linear = 0.22f;
    pointLight3.quadratic = 0.2f;
    pointLight3.intensity = 1.0f;
    
    program.lights.push_back(&pointLight);
    program.lights.push_back(&pointLight2);
    program.lights.push_back(&pointLight3);
    
    SpotLight spotlight = SpotLight();
    spotlight.direction = glm::normalize(glm::vec3(0.0f, 2.0f, 1.0f));
    spotlight.intensity = 1.0f;
    spotlight.lightColor = glm::vec3(1.0f,1.0f,1.0f);
    spotlight.innerCutOff = 0.97;
    spotlight.outerCutOff = 0.96;
    spotlight.pos = glm::vec3(0.0f,0.0f,3.0f);
    
    program.lights.push_back(&spotlight);
    
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
        
        pointLight.pos.x = sin(time);
        pointLight.pos.z = cos(time);
        
        spotlight.pos = camera.pos;
        spotlight.direction = -camera.getLookingDirection();
        
        program.render();
        
        program.endRenderLoop();
    }
    glfwTerminate();
    return 0;
}
