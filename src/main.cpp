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

  Texture* brickwallDiffuse = program.resourcesManager.loadTexture(
      "texture_diffuse", "./assets/brickwall.jpg", GL_SRGB);
  Texture* brickwallNormal = program.resourcesManager.loadTexture(
      "texture_normal", "./assets/brickwall_normal.jpg", GL_RGB);

  Texture* containerDiffuse = program.resourcesManager.loadTexture(
      "texture_diffuse", "./assets/container.png", GL_SRGB);
  Texture* containerSpecular = program.resourcesManager.loadTexture(
      "texture_specular", "./assets/container_specular.png", GL_RGB);
  Texture* containerEmission = program.resourcesManager.loadTexture(
      "texture_emission", "./assets/container_emission.png", GL_SRGB);

  CubeMesh* cubeMesh = new CubeMesh();
  program.resourcesManager.loadMesh(cubeMesh);

  std::vector<Texture*> brickWallTextures = {brickwallDiffuse, brickwallNormal};

  std::vector<Texture*> containerTextures = {
      containerDiffuse, containerSpecular, containerEmission};

  Material* brickwallMaterial = program.resourcesManager.loadMaterial(
      "brickwallMaterial", 64, brickWallTextures);

  Material* containerMaterial = program.resourcesManager.loadMaterial(
      "containerMaterial", 64, containerTextures);

  Model* model = program.resourcesManager.loadModel(
      "./assets/geralt/scene.gltf", "Geralt", false);
  model->shader = uberShader;
  model->position = {0.0f, -0.6f, 0.0f};
  model->rotate({-90.0f, 0.0f, 0.0f});

  program.renderer.initialize(program.resourcesManager.meshes);

  Model floor = Model(cubeMesh, brickwallMaterial);
  floor.shader = uberShader;
  floor.position = glm::vec3(0.0, -1.0f, 0.0f);
  floor.scale = glm::vec3(10.0f, 0.1f, 10.0f);
  floor.tileTextures = true;

  Model container = Model(cubeMesh, containerMaterial);
  container.shader = uberShader;
  container.position = {-3.0f, 0.0f, 0.0f};

  Node scene = Node();
  scene.addChild(&floor);
  scene.addChild(&container);
  scene.addChild(model);

  DirectionalLight light = DirectionalLight(
      glm::vec3(1.0f, 1.0f, 1.0f), glm::normalize(glm::vec3(1.0f, -2.0f, 0.0f)),
      1.0f, 0.5f);

  // scene.addChild(&light);

  PointLight pointLight =
      PointLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 3.0f, 3.0f), 2.0f,
                 0.1f, 1.0f, 0.14f, 0.07f);
  PointLight pointLight2 =
      PointLight(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 2.0f,
                 0.1f, 1.0f, 0.14f, 0.07f);
  PointLight pointLight3 =
      PointLight(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 2.0f,
                 0.1f, 1.0f, 0.14f, 0.07f);

  scene.addChild(&pointLight);
  scene.addChild(&pointLight2);
  scene.addChild(&pointLight3);

  SpotLight spotlight = SpotLight(
      glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),
      glm::normalize(glm::vec3(2.0f, -1.0f, 0.0f)), 1.0f, 0.1f, 0.96f, 0.94f);

  // scene.addChild(&spotlight);

  Camera camera =
      Camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  program.selectedCamera = &camera;

  program.mainNode = &scene;

  // render loop
  while (!program.shouldWindowClose()) {
    auto [time, delta] = program.startRenderLoop();

    model->rotate({0.0f, -30.0f * delta, 0.0f});

    pointLight.info.pos.x = sin(time) * 3;
    pointLight.info.pos.z = cos(time) * 3;

    pointLight2.info.pos.x = sin(time * 2 + 1) * 3;
    pointLight2.info.pos.y = cos(time * 2 + 1) * 3;

    pointLight3.info.pos.z = sin(time * 2 + 2) * 3;
    pointLight3.info.pos.y = cos(time * 2 + 2) * 3;

    spotlight.info.pos = camera.position;
    spotlight.info.direction = camera.getBasisForward();

    program.endRenderLoop();
  }
  glfwTerminate();
  delete model;
  return 0;
}
