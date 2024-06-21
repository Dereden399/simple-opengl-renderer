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
  renderer = Renderer();
  resourcesManager = ResourcesManager();
  resourcesManager.setRenderer(&renderer);
  mainNode = nullptr;
  _exposure = 1.0f;
  _lastFrame = 0.0f;
};

void Program::_framebuffer_size_callback(GLFWwindow* window, int width,
                                         int height) {
  glViewport(0, 0, width, height);
  std::cout << "Changed viewport size: " << width << "x" << height << std::endl;
  if (selectedCamera != nullptr) {
    selectedCamera->setAspectRatio((float)width / height);
  }
};

int Program::initialise() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  _window = glfwCreateWindow(800, 600, "SimpleOpenGLRenderer", NULL, NULL);
  if (_window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(_window);
  glfwSetWindowUserPointer(_window, this);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glfwSetFramebufferSizeCallback(
      _window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        Program* progPtr =
            reinterpret_cast<Program*>(glfwGetWindowUserPointer(window));
        if (progPtr) {
        }
      });
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_FRAMEBUFFER_SRGB);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // loading default textures
  resourcesManager.initialize();

  _initialized = true;
  return 0;
};

std::pair<float, float> Program::startRenderLoop() {
  glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  float curFrame = glfwGetTime();
  _deltaTime = curFrame - _lastFrame;
  _lastFrame = curFrame;
  _handleInput();
  return std::make_pair(curFrame, _deltaTime);
}

void Program::endRenderLoop() {
  renderer.drawNode(mainNode, selectedCamera);
  renderer.applyHdr(_exposure);
  glfwSwapBuffers(_window);
  glfwPollEvents();
}

bool Program::shouldWindowClose() { return glfwWindowShouldClose(_window); }

void Program::_handleInput() {
  if (selectedCamera == nullptr) return;
  auto _forward = selectedCamera->getBasisForward();
  auto _up = selectedCamera->getBasisUp();
  auto _right = selectedCamera->getBasisRight();
  glm::vec3 mov = glm::vec3(0.0f);
  glm::vec3 rot = glm::vec3(0.0f);
  if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(_window, true);

  if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
    mov += _forward;
  else if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
    mov -= _forward;

  if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
    mov += _right;
  else if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
    mov -= _right;

  if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    mov -= _up;
  else if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    mov += _up;

  if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    rot.y -= 1;
  else if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS)
    rot.y += 1;

  if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS)
    rot.x -= 1;
  else if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS)
    rot.x += 1;

  if (glfwGetKey(_window, GLFW_KEY_EQUAL) == GLFW_PRESS)
    _exposure += 0.1f;
  else if (glfwGetKey(_window, GLFW_KEY_MINUS) == GLFW_PRESS)
    _exposure -= 0.1f;

  if (mov != glm::vec3(0.0f))
    selectedCamera->move(glm::normalize(mov) * _deltaTime * 3.0f);
  if (rot != glm::vec3(0.0f))
    selectedCamera->rotate(glm::normalize(rot) * _deltaTime * 40.0f);
}
