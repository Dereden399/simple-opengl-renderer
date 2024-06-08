//
//  Movable.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 7.6.2024.
//

#ifndef Movable_hpp
#define Movable_hpp

#include <glm/glm.hpp>

class Movable {
public:
    glm::vec3 pos;
    glm::vec3 rotation;
    glm::vec3 scale;
    
    Movable() : pos(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(glm::vec3(1.0f)) {};
    Movable(glm::vec3 pos_) : pos(pos_), rotation(glm::vec3(0.0f)), scale(glm::vec3(1.0f)) {};
    
    glm::mat4 getModelMatrix();
};

#endif /* Movable_hpp */
