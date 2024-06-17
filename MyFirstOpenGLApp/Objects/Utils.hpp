//
//  Utils.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 9.6.2024.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 text;
    
    Vertex(glm::vec3 pos_, glm::vec3 norm_ = glm::vec3(0.0f), glm::vec2 text_ = glm::vec2(0.0f)): pos(pos_), norm(norm_), text(text_) {};
    Vertex(glm::vec3 pos_, glm::vec2 text_ = glm::vec2(0.0f), glm::vec3 norm_ = glm::vec3(0.0f)): pos(pos_), norm(norm_), text(text_) {};
};

class Movable {
public:
    glm::vec3 position;
    Movable() : position(glm::vec3(0.0f)) {};
    Movable(glm::vec3 pos_): position(pos_) {};
    
    void move(glm::vec3 dir) {
        position += dir;
    };
};

class Rotatable {
protected:
    glm::vec3 _rotation;
    glm::vec3 _forward;
    glm::vec3 _up;
    glm::vec3 _right;
    
    void updateBasis();
    glm::mat3 getRotationMatrix3x3();
public:
    Rotatable() : _rotation(glm::vec3(0.0f)) {updateBasis();};
    Rotatable(glm::vec3 rot_) : _rotation(rot_) {updateBasis();};
    glm::mat4 getRotationMatrix() {return getRotationMatrix(_rotation);};
    glm::mat4 getRotationMatrix(glm::vec3 angles);
    
    glm::vec3 getBasisForward() const {return _forward;};
    glm::vec3 getBasisUp() const {return _up;};
    glm::vec3 getBasisRight() const {return _right;};
    
    void rotate(glm::vec3 angles);
    void setRotation(glm::vec3 angles);
    void faceTo(const glm::vec3& dir);
};

class Scalable {
public:
    glm::vec3 scale;
    Scalable(): scale(glm::vec3(1.0f)) {};
    Scalable(glm::vec3 scale_): scale(scale_) {};
};

#endif /* Utils_hpp */
