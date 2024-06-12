//
//  Material.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 12.6.2024.
//

#ifndef Material_hpp
#define Material_hpp

#include <glm/glm.hpp>
#include "../Texture.hpp"
#include <string>

class Material {
public:
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
    Texture* diffuseMap;
    Texture* normalMap;
    Texture* specularMap;
    std::string name;
    Material(std::string name_, glm::vec3 diffuseColor_, glm::vec3 specularColor_, float shininess_): name(name_), diffuseColor(diffuseColor_), specularColor(specularColor_), shininess(shininess_) {
        diffuseMap = nullptr;
        normalMap = nullptr;
        specularMap = nullptr;
    };
    Material(std::string name_, Texture* diffuseMap_, Texture* specularMap_, float shininess_): name(name_), diffuseMap(diffuseMap_), specularMap(specularMap_), shininess(shininess_) {};
};

#endif /* Material_hpp */
