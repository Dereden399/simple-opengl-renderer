//
//  Object.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 6.6.2024.
//

#ifndef Object_hpp
#define Object_hpp

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Vertex.hpp"
#include "../Texture.hpp"
#include "Mesh.hpp"
#include "Movable.hpp"

class Object : public Movable {
public:
    Mesh* mesh;
    Texture* texture;
    glm::vec3 color;
    float textureMix;
    
    Object(Mesh* m_ = NULL, Texture* t_ = NULL, glm::vec3 p_ = glm::vec3(0.0f)) : Movable(p_) {
        mesh = m_;
        texture = t_;
        color = glm::vec3(0.0f);
        textureMix = t_ != NULL ? 1.0f : 0.0f;
    };
    Object(Mesh* mesh_, glm::vec3 pos_) : Movable(pos_), mesh(mesh_) {texture = NULL;};
    
    void setMesh(Mesh* mesh_);
    void setTexture(Texture* text) {texture = text;};
};

#endif /* Object_hpp */
