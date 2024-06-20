//
//  Program.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 9.6.2024.
//

#ifndef Program_hpp
#define Program_hpp

#include "ResourcesManager.hpp"
#include "Objects/Node.hpp"
#include <string>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>

class Program {
public:
    Camera* selectedCamera;
    Renderer renderer;
    ResourcesManager resourcesManager;
    Node* mainNode;
    
    Program();
    
    int initialise();
    void endRenderLoop();
    std::pair<float, float> startRenderLoop();
    bool shouldWindowClose();
private:
    bool _initialized = false;
    GLFWwindow* _window;
    void _handleInput();
    void _framebuffer_size_callback(GLFWwindow* window, int width, int height);
    float _lastFrame;
    float _deltaTime;
};

#endif /* Program_hpp */
