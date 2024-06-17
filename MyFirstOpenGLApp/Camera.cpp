//
//  Camera.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 8.6.2024.
//

#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

Camera::Camera(glm::vec3 pos_, glm::vec3 worldUp = glm::vec3(0.0f,1.0f,0.0f)): Movable(pos_), Rotatable() {
    _worldUp = worldUp;
    _fov = 45;
    updateBasis();
};

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + _forward, _worldUp);
};

glm::mat4 Camera::getProjectionMatrix() {
    glm::mat4 projection = glm::perspective(glm::radians(_fov), 800.0f / 600.0f, 0.1f, 100.0f);
    return projection;
};

glm::mat4 Camera::getProjectionViewMatrix() {
    return getProjectionMatrix()*getViewMatrix();
};
