//
//  Camera.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 8.6.2024.
//

#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos_, glm::vec3 worldUp = glm::vec3(0.0f,1.0f,0.0f)): Movable(pos_) {
    _worldUp = worldUp;
    _fov = 45;
};

void Camera::pointAt(glm::vec3 point) {
    glm::vec3 direction = glm::normalize(pos - point);
    float rotationY = glm::degrees(atan2(direction.x, direction.z));
    float rotationX = glm::degrees(atan2(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)));
    rotation.x = -rotationX;
    rotation.y = rotationY;
};

glm::mat4 Camera::getModelMatrix() {
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f,0.0f,0.0f));
    rot = glm::rotate(rot, glm::radians(rotation.y), glm::vec3(0.0f,1.0f,0.0f));
    rot = glm::rotate(rot, glm::radians(rotation.z), glm::vec3(0.0f,0.0f,1.0f));
    glm::vec3 direction = glm::vec3(rot*glm::vec4(0.0f,0.0f,-1.0f,0.0f));
    direction = glm::normalize(direction);
    return glm::lookAt(pos, pos + direction, _worldUp);
};

glm::mat4 Camera::getProjectionMatrix() {
    glm::mat4 projection = glm::perspective(glm::radians(_fov), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = getModelMatrix();
    return projection*view;
};

