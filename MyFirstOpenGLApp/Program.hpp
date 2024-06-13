//
//  Program.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 9.6.2024.
//

#ifndef Program_hpp
#define Program_hpp

#include "Shader.hpp"
#include "Renderer.hpp"
#include "Lights.hpp"
#include <string>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>

class Program {
public:
    Camera* selectedCamera;
    Renderer renderer;
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    
    Program();
    
    int initialise();
    void render();
    void endRenderLoop();
    std::pair<float, float> startRenderLoop();
    bool shouldWindowClose();
private:
    GLFWwindow* _window;
    void _handleInput();
    void _framebuffer_size_callback(GLFWwindow* window, int width, int height);
    float _lastFrame;
    float _deltaTime;
    std::map<Shader*, std::vector<Object*>> _objectsToDraw;
};

#endif /* Program_hpp */
