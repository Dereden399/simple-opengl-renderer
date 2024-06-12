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
    _up = _worldUp;
    _forward = glm::vec3(0.0f,0.0f,-1.0f);
    _right = glm::cross(_forward, _worldUp);
    rotation = glm::vec3(0.0f);
};

void Camera::pointAt(glm::vec3 point) {
    glm::vec3 direction = glm::normalize(point-pos);
    float rotationY = glm::degrees(atan2(direction.x, direction.z));
    float rotationX = glm::degrees(atan2(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)));
    rotation.x = -rotationX;
    rotation.y = 180-rotationY;
    _forward = direction;
    _right = glm::cross(_forward, _worldUp);
    _up = glm::cross(_forward, _right);
};

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(pos, pos + _forward, _worldUp);
};

glm::mat4 Camera::getProjectionMatrix() {
    glm::mat4 projection = glm::perspective(glm::radians(_fov), 800.0f / 600.0f, 0.1f, 100.0f);
    return projection;
};

glm::mat4 Camera::getProjectionViewMatrix() {
    return getProjectionMatrix()*getViewMatrix();
};

glm::vec3 Camera::getLookingDirection() {
    return _forward;
};

glm::mat3 Camera::getCameraBasis() {
    return glm::mat3(_forward, _right, _up);
}

void Camera::rotate(glm::vec3 angles) {
    rotation += angles;
    updateBasis();
};

void Camera::setRotation(glm::vec3 angles) {
    rotation = angles;
    updateBasis();
};

void Camera::updateBasis() {
    auto rotM = getRotationMatrix();
    _forward = glm::vec3(rotM*glm::vec4(glm::vec3(0.0f,0.0f,-1.0f), 0.0f));
    _up = glm::vec3(rotM*glm::vec4(_worldUp, 0.0f));
    _right = glm::cross(_forward, _up);
};

