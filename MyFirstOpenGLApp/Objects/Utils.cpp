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
