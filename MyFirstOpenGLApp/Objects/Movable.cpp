//
//  Movable.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 7.6.2024.
//

#include "Movable.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

glm::mat4 Movable::getModelMatrix() {
    glm::mat4 result = glm::scale(glm::mat4(1.0f), this->scale);
    result = glm::translate(result, this->pos);
    glm::mat4 rot = glm::mat4_cast(glm::quat(glm::vec3(glm::radians(rotation.x),glm::radians(rotation.y),glm::radians(rotation.z))));
    return result*rot;
}
