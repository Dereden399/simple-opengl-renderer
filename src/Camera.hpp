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

  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();
  glm::mat4 getProjectionViewMatrix();
  void setAspectRatio(float ratio) { _aspectRatio = ratio; };

 private:
  glm::vec3 _worldUp;
  float _fov;
  float _aspectRatio;
};

#endif /* Camera_hpp */
