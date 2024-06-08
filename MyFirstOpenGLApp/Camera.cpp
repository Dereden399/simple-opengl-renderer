//
//  Camera.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 8.6.2024.
//

#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

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
    auto direction = getLookingDirection();
    return glm::lookAt(pos, pos + direction, _worldUp);
};

glm::mat4 Camera::getProjectionMatrix() {
    glm::mat4 projection = glm::perspective(glm::radians(_fov), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = getModelMatrix();
    return projection*view;
};

glm::mat4 Camera::getRotationMatrix() {
    return glm::mat4_cast(glm::quat(glm::vec3(glm::radians(rotation.x),glm::radians(rotation.y),glm::radians(rotation.z))));
};

glm::vec3 Camera::getLookingDirection() {
    glm::mat4 rot = getRotationMatrix();
    rot = glm::rotate(rot, glm::radians(rotation.z), glm::vec3(0.0f,0.0f,1.0f));
    glm::vec3 direction = glm::vec3(rot*glm::vec4(0.0f,0.0f,-1.0f,0.0f));
    direction = glm::normalize(direction);
    return direction;
};

glm::mat3 Camera::getCameraBasis() {
    auto forward = getLookingDirection();
    auto left = glm::cross(forward, _worldUp);
    auto up = glm::cross(forward, left);
    return glm::mat3(forward, left, up);
}

