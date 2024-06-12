//
//  Camera.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 8.6.2024.
//

#ifndef Camera_hpp
#define Camera_hpp

#include "Objects/Utils.hpp"

class Camera : public Movable, public Rotatable {
public:
    Camera(glm::vec3 pos_, glm::vec3 worldUp);
    
    void pointAt(glm::vec3 point);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getProjectionViewMatrix();
    glm::vec3 getLookingDirection();
    glm::mat3 getCameraBasis();
    void rotate(glm::vec3 angles);
    void setRotation(glm::vec3 angles);
private:
    glm::vec3 _worldUp;
    glm::vec3 _forward;
    glm::vec3 _right;
    glm::vec3 _up;
    float _fov;
    
    void updateBasis();
};

#endif /* Camera_hpp */
