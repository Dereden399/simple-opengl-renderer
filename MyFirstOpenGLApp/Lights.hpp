//
//  Lights.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 12.6.2024.
//

#ifndef Lights_hpp
#define Lights_hpp

#include <glm/glm.hpp>

struct DirectionalLight {
    glm::vec3 lightColor;
    glm::vec3 direction;
    float intensity;
};

struct PointLight {
    glm::vec3 lightColor;
    glm::vec3 pos;
    float constant;
    float linear;
    float quadratic;
    float intensity;
};


#endif /* Lights_hpp */
