//
//  Lights.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 12.6.2024.
//

#ifndef Lights_hpp
#define Lights_hpp

#include <glm/glm.hpp>

// Have to declare ambientIntensity in child classes to match the glsl structure

class Light {
public:
    enum class LightType {DirLight, SpotLight, PointLight};
    glm::vec3 lightColor;
    float intensity;
    
    Light(glm::vec3 lightColor_ = glm::vec3(0.0f), float intensity_ = 1.0f): lightColor(lightColor_), intensity(intensity_) {};
    
    virtual LightType getType() const = 0;
};

class DirectionalLight : public Light {
public:
    glm::vec3 direction;
    float ambientIntensity;
    
    DirectionalLight(glm::vec3 lightColor_ = glm::vec3(0.0f), glm::vec3 direction_ = glm::vec3(0.0f,0.0f,1.0f), float intensity_ = 1.0f, float ambientIntenisty_ = 0.1f) : Light(lightColor_, intensity_), direction(direction_), ambientIntensity(ambientIntenisty_) {};
    
    LightType getType() const override {return Light::LightType::DirLight;};
    
    struct DirLightForUBO {
        glm::vec3 lightColor;
        float intensity;
        glm::vec3 direction;
        float ambientIntensity;
    };
    
    DirLightForUBO getStruct() {
        auto d = DirLightForUBO();
        d.intensity = intensity;
        d.ambientIntensity = ambientIntensity;
        d.lightColor = lightColor;
        d.direction = direction;
        return d;
    }
};

struct PointLight: public Light {
public:
    glm::vec3 pos;
    float constant;
    float linear;
    float quadratic;
    float ambientIntensity;
    
    PointLight(glm::vec3 lightColor_ = glm::vec3(0.0f), glm::vec3 pos_ = glm::vec3(0.0f), float intensity_ = 1.0f, float ambientIntenisty_ = 0.1f, float constant_ = 1.0f, float linear_ = 0.4f, float quadratic_ = 0.16f) : Light(lightColor_, intensity_), pos(pos_), constant(constant_), linear(linear_), quadratic(quadratic_), ambientIntensity(ambientIntenisty_) {};
    
    LightType getType() const override {return Light::LightType::PointLight;};
    
    struct PLightForUBO {
        glm::vec3 lightColor;
        float intensity;
        glm::vec3 pos;
        float constant;
        float linear;
        float quadratic;
        float ambientIntensity;
    };
    
    PLightForUBO getStruct() {
        auto d = PLightForUBO();
        d.intensity = intensity;
        d.ambientIntensity = ambientIntensity;
        d.lightColor = lightColor;
        d.pos = pos;
        d.constant = constant;
        d.linear = linear;
        d.quadratic = quadratic;
        return d;
    }
};

struct SpotLight: public Light {
public:
    glm::vec3 pos;
    float innerCutOff;
    glm::vec3 direction;
    float outerCutOff;
    float ambientIntensity;
    
    SpotLight(glm::vec3 lightColor_ = glm::vec3(0.0f), glm::vec3 pos_ = glm::vec3(0.0f), glm::vec3 direction_ = glm::vec3(0.0f,0.0f,1.0f), float intensity_ = 1.0f, float ambientIntenisty_ = 0.1f, float innerCutOff_ = 0.97f, float outerCutOff_ = 0.96f) : Light(lightColor_, intensity_), pos(pos_), direction(direction_), innerCutOff(innerCutOff_), outerCutOff(outerCutOff_), ambientIntensity(ambientIntenisty_) {};
    
    LightType getType() const override {return Light::LightType::SpotLight;};
    
    struct SLightForUBO {
        glm::vec3 lightColor;
        float intensity;
        glm::vec3 pos;
        float innerCutOff;
        glm::vec3 direction;
        float outerCutOff;
        float ambientIntensity;
    };
    
    SLightForUBO getStruct() {
        auto d = SLightForUBO();
        d.intensity = intensity;
        d.ambientIntensity = ambientIntensity;
        d.lightColor = lightColor;
        d.pos = pos;
        d.innerCutOff = innerCutOff;
        d.direction = direction;
        d.outerCutOff = outerCutOff;
        return d;
    }
};

#endif /* Lights_hpp */
