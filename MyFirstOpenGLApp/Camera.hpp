//
//  Camera.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 8.6.2024.
//

#ifndef Camera_hpp
#define Camera_hpp

#include "Objects/Movable.hpp"

class Camera : public Movable {
public:
    Camera(glm::vec3 pos_, glm::vec3 worldUp);
    glm::mat4 getModelMatrix() override;
    void pointAt(glm::vec3 point);
    glm::mat4 getProjectionMatrix();
private:
    glm::vec3 _worldUp;
    float _fov;
};

#endif /* Camera_hpp */
