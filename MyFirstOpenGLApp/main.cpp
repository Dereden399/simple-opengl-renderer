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
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window, float delta);

Camera camera = Camera(glm::vec3(0.0f,0.0f,3.0f), glm::vec3(0.0f,1.0f,0.0f));


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glEnable(GL_DEPTH_TEST);
    
    Shader shaders = Shader("./Shaders/vertexShader.glsl", "./Shaders/fragmentShader.glsl");
    Texture boxTexture = Texture("Assets/container.jpg", "boxTexture", GL_RGB, GL_RGB, false, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_TEXTURE0);
    Texture faceTexture = Texture("Assets/awesomeface.png", "faceTexture", GL_RGB, GL_RGBA, true, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_TEXTURE0);
    
    CubeMesh cubeMesh = CubeMesh();
    
    std::vector<Object> objects;
    objects.push_back(Object(&cubeMesh, &boxTexture));
    objects.push_back(Object(&cubeMesh, &faceTexture, glm::vec3(1.5f, 1.5f, 0.0f)));
    objects[0].color = glm::vec3(1.0f,1.0f, 0.0f);
    objects[0].textureMix = 0.9f;
    
    Renderer renderer = Renderer(shaders);
    renderer.addStaticMesh(&cubeMesh);
    renderer.initialise();
    
    shaders.use();
    shaders.setUniform("texture0", {0});
    
    float deltaTime = 0.0f;
    float lastFrame = glfwGetTime();
    // render loop
    while(!glfwWindowShouldClose(window)) {
        float curFrame = glfwGetTime();
        deltaTime = curFrame - lastFrame;
        lastFrame = curFrame;
        processInput(window, deltaTime);
        
        auto projectionView = camera.getProjectionMatrix();
        shaders.setUniform("projectionView", projectionView);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        objects[0].rotation.x += deltaTime*30;
        objects[0].rotation.y += deltaTime*30;
        objects[0].pos.y = sin(curFrame)/4;
        
        renderer.drawObjects(objects);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, float delta)
{
    glm::vec3 mov = glm::vec3(0.0f);
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mov.z = -1;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mov.z = 1;
    
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mov.x = -1;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mov.x = 1;
    
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        mov.y = -1;
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        mov.y = 1;
    
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.rotation.y -= delta*30.0f;
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.rotation.y += delta*30.0f;
    
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.rotation.x += delta*30.0f;
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.rotation.x -= delta*30.0f;
    
    
    if (mov != glm::vec3(0.0f))
        camera.pos += glm::normalize(mov)*delta*2.0f;
}
