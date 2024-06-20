//
//  Lights.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 12.6.2024.
//

#ifndef Lights_hpp
#define Lights_hpp

#include <glm/glm.hpp>
#include "Objects/Node.hpp"

// Have to declare ambientIntensity in child classes to match the glsl structure

class Light : public Node {
 public:
  enum class LightType { DirLight, SpotLight, PointLight };

  Light() : Node(){};

  virtual LightType getType() const = 0;

  NodeType getNodeType() override { return NodeType::Light; };
};

class DirectionalLight : public Light {
 public:
  struct {
    glm::vec3 lightColor;
    float intensity;
    glm::vec3 direction;
    float ambientIntensity;
  } info;

  DirectionalLight(glm::vec3 lightColor_ = glm::vec3(0.0f),
                   glm::vec3 direction_ = glm::vec3(0.0f, 0.0f, 1.0f),
                   float intensity_ = 1.0f, float ambientIntenisty_ = 0.1f)
      : Light(),
        info({lightColor_, intensity_, direction_, ambientIntenisty_}){};

  LightType getType() const override { return Light::LightType::DirLight; };
};

struct PointLight : public Light {
 public:
  struct {
    glm::vec3 lightColor;
    float intensity;
    glm::vec3 pos;
    float constant;
    float linear;
    float quadratic;
    float ambientIntensity;
  } info;
  PointLight(glm::vec3 lightColor_ = glm::vec3(0.0f),
             glm::vec3 pos_ = glm::vec3(0.0f), float intensity_ = 1.0f,
             float ambientIntenisty_ = 0.1f, float constant_ = 1.0f,
             float linear_ = 0.4f, float quadratic_ = 0.16f)
      : Light(),
        info({lightColor_, intensity_, pos_, constant_, linear_, quadratic_,
              ambientIntenisty_}){};

  LightType getType() const override { return Light::LightType::PointLight; };
};

struct SpotLight : public Light {
 public:
  struct {
    glm::vec3 lightColor;
    float intensity;
    glm::vec3 pos;
    float innerCutOff;
    glm::vec3 direction;
    float outerCutOff;
    float ambientIntensity;
  } info;

  SpotLight(glm::vec3 lightColor_ = glm::vec3(0.0f),
            glm::vec3 pos_ = glm::vec3(0.0f),
            glm::vec3 direction_ = glm::vec3(0.0f, 0.0f, 1.0f),
            float intensity_ = 1.0f, float ambientIntenisty_ = 0.1f,
            float innerCutOff_ = 0.97f, float outerCutOff_ = 0.96f)
      : Light(),
        info({lightColor_, intensity_, pos_, innerCutOff_, direction_,
              outerCutOff_, ambientIntenisty_}){};

  LightType getType() const override { return Light::LightType::SpotLight; };
};

#endif /* Lights_hpp */
