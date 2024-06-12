//
//  Object.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 6.6.2024.
//

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Object.hpp"

glm::mat4 Object::getModelMatrix() {
    glm::mat4 result = glm::translate(glm::mat4(1.0f), this->pos);
    result = glm::scale(result, this->scale);
    return result*getRotationMatrix();
};
