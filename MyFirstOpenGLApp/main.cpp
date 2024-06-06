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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

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
    Texture boxTexture = Texture("Assets/container.jpg", "texture0", GL_RGB, GL_RGB, false, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_TEXTURE0);
    Texture faceTexture = Texture("Assets/awesomeface.png", "texture1", GL_RGB, GL_RGBA, true, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_TEXTURE1);
    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    auto projectionView = projection*view;
    std::vector<Object> objects;
    objects.emplace_back(
                         std::vector<Vertex>{
                             // Back face
                             Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f),
                             Vertex( 0.5f, -0.5f, -0.5f, 1.0f, 0.0f),
                             Vertex( 0.5f,  0.5f, -0.5f, 1.0f, 1.0f),
                             Vertex(-0.5f,  0.5f, -0.5f, 0.0f, 1.0f),
                             
                             // Front face
                             Vertex(-0.5f, -0.5f,  0.5f, 0.0f, 0.0f),
                             Vertex( 0.5f, -0.5f,  0.5f, 1.0f, 0.0f),
                             Vertex( 0.5f,  0.5f,  0.5f, 1.0f, 1.0f),
                             Vertex(-0.5f,  0.5f,  0.5f, 0.0f, 1.0f),
                             
                             // Left face
                             Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f),
                             Vertex(-0.5f,  0.5f, -0.5f, 1.0f, 0.0f),
                             Vertex(-0.5f,  0.5f,  0.5f, 1.0f, 1.0f),
                             Vertex(-0.5f, -0.5f,  0.5f, 0.0f, 1.0f),
                             
                             // Right face
                             Vertex( 0.5f, -0.5f, -0.5f, 0.0f, 0.0f),
                             Vertex( 0.5f,  0.5f, -0.5f, 1.0f, 0.0f),
                             Vertex( 0.5f,  0.5f,  0.5f, 1.0f, 1.0f),
                             Vertex( 0.5f, -0.5f,  0.5f, 0.0f, 1.0f),
                             
                             // Bottom face
                             Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f),
                             Vertex( 0.5f, -0.5f, -0.5f, 1.0f, 0.0f),
                             Vertex( 0.5f, -0.5f,  0.5f, 1.0f, 1.0f),
                             Vertex(-0.5f, -0.5f,  0.5f, 0.0f, 1.0f),
                             
                             // Top face
                             Vertex(-0.5f,  0.5f, -0.5f, 0.0f, 0.0f),
                             Vertex( 0.5f,  0.5f, -0.5f, 1.0f, 0.0f),
                             Vertex( 0.5f,  0.5f,  0.5f, 1.0f, 1.0f),
                             Vertex(-0.5f,  0.5f,  0.5f, 0.0f, 1.0f)
                         },
                         std::vector<unsigned int>{
                             // Back face
                             0, 1, 2, 2, 3, 0,
                             // Front face
                             4, 5, 6, 6, 7, 4,
                             // Left face
                             8, 9, 10, 10, 11, 8,
                             // Right face
                             12, 13, 14, 14, 15, 12,
                             // Bottom face
                             16, 17, 18, 18, 19, 16,
                             // Top face
                             20, 21, 22, 22, 23, 20}
                         );
    
    shaders.use();
    shaders.setUniform("texture0", {0});
    unsigned int projectionViewLoc = glGetUniformLocation(shaders.ID, "projectionView");
    glUniformMatrix4fv(projectionViewLoc, 1, GL_FALSE, glm::value_ptr(projectionView));
    
    // render loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        boxTexture.bind();
        //faceTexture.bind();
        shaders.use();
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        unsigned int modelLoc = glGetUniformLocation(shaders.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        for (auto& obj : objects) {
            obj.draw();
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
