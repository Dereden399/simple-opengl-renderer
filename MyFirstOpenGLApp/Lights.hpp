//
//  Lights.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 12.6.2024.
//

#ifndef Lights_hpp
#define Lights_hpp

#include <glm/glm.hpp>

class Light {
public:
    enum class LightType {DirLight, SpotLight, PointLight};
    glm::vec3 lightColor;
    float intensity;
    float ambientIntensity;
    
    Light(glm::vec3 lightColor_ = glm::vec3(0.0f), float intensity_ = 1.0f, float ambientIntenisty_ = 0.1f): lightColor(lightColor_), intensity(intensity_), ambientIntensity(ambientIntenisty_) {};
    
    virtual LightType getType() const = 0;
};

class DirectionalLight : public Light {
public:
    glm::vec3 direction;
    
    DirectionalLight(glm::vec3 lightColor_ = glm::vec3(0.0f), glm::vec3 direction_ = glm::vec3(0.0f,0.0f,1.0f), float intensity_ = 1.0f, float ambientIntenisty_ = 0.1f) : Light(lightColor_, intensity_, ambientIntenisty_), direction(direction_) {};
    
    LightType getType() const override {return Light::LightType::DirLight;};
};

struct PointLight: public Light {
public:
    glm::vec3 pos;
    float constant;
    float linear;
    float quadratic;
    
    PointLight(glm::vec3 lightColor_ = glm::vec3(0.0f), glm::vec3 pos_ = glm::vec3(0.0f), float intensity_ = 1.0f, float ambientIntenisty_ = 0.1f, float constant_ = 1.0f, float linear_ = 0.4f, float quadratic_ = 0.16f) : Light(lightColor_, intensity_, ambientIntenisty_), pos(pos_), constant(constant_), linear(linear_), quadratic(quadratic_) {};
    
    LightType getType() const override {return Light::LightType::PointLight;};
};

struct SpotLight: public Light {
public:
    glm::vec3 pos;
    glm::vec3 direction;
    float innerCutOff;
    float outerCutOff;
    
    SpotLight(glm::vec3 lightColor_ = glm::vec3(0.0f), glm::vec3 pos_ = glm::vec3(0.0f), glm::vec3 direction_ = glm::vec3(0.0f,0.0f,1.0f), float intensity_ = 1.0f, float ambientIntenisty_ = 0.1f, float innerCutOff_ = 0.97f, float outerCutOff_ = 0.96f) : Light(lightColor_, intensity_, ambientIntenisty_), pos(pos_), direction(direction_), innerCutOff(innerCutOff_), outerCutOff(outerCutOff_) {};
    
    LightType getType() const override {return Light::LightType::SpotLight;};
};

#endif /* Lights_hpp */
