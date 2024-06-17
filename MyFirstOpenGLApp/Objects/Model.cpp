//
//  Model.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 16.6.2024.
//

#include "Model.hpp"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Model::getModelMatrix() {
    glm::mat4 result = glm::translate(glm::mat4(1.0f), this->position);
    result = glm::scale(result, this->scale);
    return result*getRotationMatrix();
};
