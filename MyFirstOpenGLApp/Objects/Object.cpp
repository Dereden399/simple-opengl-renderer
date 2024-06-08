//
//  Object.cpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 6.6.2024.
//

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Object.hpp"

void Object::setMesh(Mesh* mesh_) {
    if (mesh != NULL) {
        std::cout << "ERROR::REASSIGNING::MESH" << std::endl;
        return;
    }
    mesh = mesh_;
};
