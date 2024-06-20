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
  std::string name;
  std::vector<Texture*> textures;
  glm::vec3 blendColor;
  float shininess;
  Material(std::string name_, float shininess_,
           std::vector<Texture*>& textures_,
           glm::vec3 blendColor_ = glm::vec3(1.0f))
      : name(name_),
        textures(textures_),
        blendColor(blendColor_),
        shininess(shininess_){};
};

#endif /* Material_hpp */
