//
//  main.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 25.5.2024.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Camera.hpp"
#include "Lights.hpp"
#include "Objects/Material.hpp"
#include "Objects/Model.hpp"
#include "Program.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

int main() {
  Program program = Program();
  if (program.initialise() == -1) {
    return -1;
  }
  Shader* uberShader = program.resourcesManager.loadShader(
      "./shaders/vertexShader.glsl", "./shaders/BlinnPhongFragment.glsl");
  Shader* hdrShader = program.resourcesManager.loadShader(
      "./shaders/hdrVertex.glsl", "./shaders/hdrFragment.glsl");
  Shader* blurShader = program.resourcesManager.loadShader(
      "./shaders/hdrVertex.glsl", "./shaders/BlurFragment.glsl");
  program.renderer.setHdrShader(hdrShader);
  program.renderer.setBlurShader(blurShader);
  Texture* brickwallDiffuse = program.resourcesManager.loadTexture(
      "texture_diffuse", "./assets/brickwall.jpg", GL_SRGB);
  Texture* brickwallNormal = program.resourcesManager.loadTexture(
      "texture_normal", "./assets/brickwall_normal.jpg", GL_RGB);

  Texture* containerDiffuse = program.resourcesManager.loadTexture(
      "texture_diffuse", "./assets/container.png", GL_SRGB);
  Texture* containerSpecular = program.resourcesManager.loadTexture(
      "texture_specular", "./assets/container_specular.png", GL_RGB);
  // Texture* containerEmission = program.resourcesManager.loadTexture(
  //     "texture_emission", "./assets/container_emission.png", GL_SRGB);

  CubeMesh* cubeMesh = new CubeMesh();
  program.resourcesManager.loadMesh(cubeMesh);

  std::vector<Texture*> brickWallTextures = {brickwallDiffuse, brickwallNormal};

  std::vector<Texture*> containerTextures = {containerDiffuse,
                                             containerSpecular};

  Material* brickwallMaterial = program.resourcesManager.loadMaterial(
      "brickwallMaterial", 64, brickWallTextures);

  Material* containerMaterial = program.resourcesManager.loadMaterial(
      "containerMaterial", 64, containerTextures);

  Model* model = program.resourcesManager.loadModel(
      "./assets/geralt/scene.gltf", "Geralt", false);
  model->shader = uberShader;
  model->position = {2.0f, -0.5f, 0.0f};
  model->scale = {1.5f, 1.5f, 1.5f};
  model->rotate({-90.0f, 0.0f, 0.0f});

  Model* bonfire = program.resourcesManager.loadModel(
      "./assets/bonfire/scene.gltf", "Bonfire", false, 100.0f);
  bonfire->shader = uberShader;
  bonfire->position = {0.0f, 0.16f, 0.0f};
  bonfire->scale = {1.2f, 1.2f, 1.2f};

  program.renderer.initialize(program.resourcesManager.meshes,
                              program.resourcesManager.renderMesh);
  Model floor = Model(cubeMesh, brickwallMaterial);
  floor.shader = uberShader;
  floor.position = glm::vec3(0.0, -0.6f, 0.0f);
  floor.scale = glm::vec3(10.0f, 0.1f, 10.0f);
  floor.tileTextures = true;

  Model container = Model(cubeMesh, containerMaterial);
  container.shader = uberShader;
  container.position = {0.0f, 0.5f, 3.0f};

  Node scene = Node();
  scene.addChild(&floor);
  scene.addChild(&container);
  scene.addChild(model);
  scene.addChild(bonfire);

  DirectionalLight light = DirectionalLight(
      glm::vec3(1.0f, 1.0f, 1.0f), glm::normalize(glm::vec3(1.0f, -2.0f, 0.0f)),
      1.0f, 0.5f);

  // scene.addChild(&light);

  PointLight pointLight =
      PointLight(glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f,
                 0.1f, 1.0f, 0.2f, 0.5f);

  scene.addChild(&pointLight);

  Camera camera =
      Camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  program.selectedCamera = &camera;

  program.mainNode = &scene;

  // render loop
  while (!program.shouldWindowClose()) {
    auto [time, delta] = program.startRenderLoop();

    model->rotate({0.0f, -30.0f * delta, 0.0f});
    pointLight.info.quadratic = 0.5f + 0.3f * sin(time);

    program.endRenderLoop();
  }
  glfwTerminate();
  delete model;
  delete bonfire;
  return 0;
}
