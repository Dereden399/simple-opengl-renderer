//
//  Program.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 9.6.2024.
//

#include "Program.hpp"
#include <iostream>
#include <glad/glad.h>

Program::Program() {
    selectedCamera = nullptr;
    _window = nullptr;
    lights = std::vector<Light*>();
    renderer = Renderer();
    _lastFrame = 0.0f;
};

Renderer::Renderer(Renderer&& r)
    : _VAO(r._VAO), _VBO(r._VBO), _EBO(r._EBO), _initialised(r._initialised), _meshes(std::move(r._meshes)) {
    // Leave the source object in a valid state
    r._VAO = 0;
    r._VBO = 0;
    r._EBO = 0;
    r._initialised = false;
}

Renderer& Renderer::operator=(Renderer&& r) {
    if (this != &r) {
        // Clean up existing resources if initialised
        if (_initialised) {
            glDeleteVertexArrays(1, &_VAO);
            glDeleteBuffers(1, &_VBO);
            glDeleteBuffers(1, &_EBO);
        }

        // Move data from the source object
        _VAO = r._VAO;
        _VBO = r._VBO;
        _EBO = r._EBO;
        _initialised = r._initialised;
        _meshes = std::move(r._meshes);

        // Leave the source object in a valid state
        r._VAO = 0;
        r._VBO = 0;
        r._EBO = 0;
        r._initialised = false;
    }
    return *this;
}

void Program::_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    if (selectedCamera != nullptr) {}
};

int Program::initialise() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    _window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(_window);
    glfwSetWindowUserPointer(_window, this);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        Program* progPtr = reinterpret_cast<Program *>(glfwGetWindowUserPointer(window));
        if (progPtr) {}
    });
    glEnable(GL_DEPTH_TEST);
    return 0;
};

std::pair<float, float> Program::startRenderLoop() {
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _objectsToDraw.clear();
    float curFrame = glfwGetTime();
    _deltaTime = curFrame - _lastFrame;
    _lastFrame = curFrame;
    _handleInput();
    return std::make_pair(curFrame, _deltaTime);
}

void Program::render() {
    for (auto& obj : objects) {
        _objectsToDraw[obj->shader].push_back(obj);
    }
}

void Program::endRenderLoop() {
    for (auto& obj : _objectsToDraw) {
        renderer.drawObjects(obj.first, obj.second, selectedCamera, lights);
    }
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

bool Program::shouldWindowClose() {
    return glfwWindowShouldClose(_window);
}

void Program::_handleInput() {
    if (selectedCamera == nullptr) return;
    auto cameraBasis = selectedCamera->getCameraBasis();
    glm::vec3 mov = glm::vec3(0.0f);
    glm::vec3 rot = glm::vec3(0.0f);
    if(glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, true);
    
    if(glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        mov += glm::vec3(cameraBasis[0]);
    else if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        mov -= glm::vec3(cameraBasis[0]);
    
    if(glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        mov -= glm::vec3(cameraBasis[1]);
    else if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        mov += glm::vec3(cameraBasis[1]);
    
    if(glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        mov -= glm::vec3(cameraBasis[2]);
    else if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        mov += glm::vec3(cameraBasis[2]);
    
    if(glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rot.y -= 1;
    else if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rot.y += 1;
    
    if(glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS)
        rot.x += 1;
    else if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS)
        rot.x -= 1;
    
    
    if (mov != glm::vec3(0.0f))
        selectedCamera->move(glm::normalize(mov)*_deltaTime*3.0f);
    if (rot != glm::vec3(0.0f))
        selectedCamera->rotate(glm::normalize(rot)*_deltaTime*40.0f);
}
