//
//  Utils.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 9.6.2024.
//

#include "Utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

glm::mat4 Rotatable::getRotationMatrix(glm::vec3 angles) {
    return glm::mat4_cast(
                          glm::quat(
                                    glm::vec3(
                                              glm::radians(angles.x),
                                              glm::radians(angles.y),
                                              glm::radians(angles.z)
                                              )
                                    )
                          );
}

glm::mat3 Rotatable::getRotationMatrix3x3() {
    return glm::mat3_cast(
                          glm::quat(
                                    glm::vec3(
                                              glm::radians(_rotation.x),
                                              glm::radians(_rotation.y),
                                              glm::radians(_rotation.z)
                                              )
                                    )
                          );
};

void Rotatable::faceTo(const glm::vec3& dir) {
    glm::vec3 direction = glm::normalize(dir);
    float rotationY = glm::degrees(atan2(direction.x, direction.z));
    float rotationX = glm::degrees(atan2(-direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)));
    // ensure that x rotation is in the range
    if (rotationX < -180.0f) rotationX += 360.0f;
    if (rotationX > 180.0f) rotationX -= 360.0f;
    _rotation.x = rotationX;
    _rotation.y = rotationY;
    _rotation.z = 0;
    updateBasis();
};

void Rotatable::rotate(glm::vec3 angles) {
    _rotation += angles;
    updateBasis();
}

void Rotatable::setRotation(glm::vec3 angles) {
    _rotation = angles;
    updateBasis();
}

void Rotatable::updateBasis() {
    glm::mat3 rot = getRotationMatrix3x3();
    _forward = rot*glm::vec3(0.0f,0.0f,1.0f);
    _up = rot*glm::vec3(0.0f,1.0f, 0.0f);
    _right = rot*glm::vec3(1.0f,0.0f,0.0f);
};

glm::mat4 Transform::getModelMatrix() {
    glm::mat4 result = glm::translate(glm::mat4(1.0f), this->position);
    result = glm::scale(result, this->scale);
    return result*getRotationMatrix();
    
}
