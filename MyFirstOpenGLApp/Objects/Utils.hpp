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
    float x,y,z;
    float u,w;
    float nx,ny,nz;
    
    Vertex(float x_, float y_, float z_) : x(x_), y(y_), z(z_), u(0), w(0), nx(0), ny(0), nz(0) {};
    Vertex(float x_, float y_, float z_, float u_, float w_) : x(x_), y(y_), z(z_), u(u_), w(w_), nx(0), ny(0), nz(0) {};
    Vertex(float x_, float y_, float z_, float u_, float w_, float nx_, float ny_, float nz_) : x(x_), y(y_), z(z_), u(u_), w(w_), nx(nx_), ny(ny_), nz(nz_) {};
};

class Movable {
public:
    glm::vec3 pos;
    Movable() : pos(glm::vec3(0.0f)) {};
    Movable(glm::vec3 pos_): pos(pos_) {};
    
    void move(glm::vec3 dir) {
        pos += dir;
    };
};

class Rotatable {
public:
    glm::vec3 rotation;
    Rotatable() : rotation(glm::vec3(0.0f)) {};
    Rotatable(glm::vec3 rot_) : rotation(rot_) {};
    
    glm::mat4 getRotationMatrix() {return getRotationMatrix(rotation);};
    glm::mat4 getRotationMatrix(glm::vec3 angles);
};

class Scalable {
public:
    glm::vec3 scale;
    Scalable(): scale(glm::vec3(1.0f)) {};
    Scalable(glm::vec3 scale_): scale(scale_) {};
};

#endif /* Utils_hpp */
