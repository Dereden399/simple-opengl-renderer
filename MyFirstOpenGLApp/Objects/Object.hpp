//
//  Object.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 6.6.2024.
//

#ifndef Object_hpp
#define Object_hpp

#include <glm/glm.hpp>
#include <string>

#include "Mesh.hpp"
#include "Utils.hpp"
#include "Material.hpp"
#include "../Shader.hpp"

class Object : public Movable, public Rotatable, public Scalable {
public:
    Shader* shader;
    Mesh* mesh;
    Material* material;
    
    Object(Mesh* m_, glm::vec3 p_ = glm::vec3(0.0f)) : Movable(p_), Rotatable(), Scalable(), mesh(m_), shader(nullptr), material(nullptr) {};
    
    glm::mat4 getModelMatrix();
};

#endif /* Object_hpp */
