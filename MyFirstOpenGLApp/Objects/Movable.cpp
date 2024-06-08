//
//  Movable.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 7.6.2024.
//

#include "Movable.hpp"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Movable::getModelMatrix() {
    glm::mat4 result = glm::scale(glm::mat4(1.0f), this->scale);
    result = glm::translate(result, this->pos);
    result = glm::rotate(result, glm::radians(this->rotation.x), glm::vec3(1.0f,0.0f,0.0f));
    result = glm::rotate(result, glm::radians(this->rotation.y), glm::vec3(0.0f,1.0f,0.0f));
    result = glm::rotate(result, glm::radians(this->rotation.z), glm::vec3(0.0f,0.0f,1.0f));
    return result;
}
