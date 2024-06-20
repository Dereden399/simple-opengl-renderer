//
//  Model.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 16.6.2024.
//

#ifndef Model_hpp
#define Model_hpp

#include <glm/glm.hpp>
#include <string>

#include "Mesh.hpp"
#include "Node.hpp"
#include "Utils.hpp"
#include "Material.hpp"
#include "../Shader.hpp"

class Model : public Node {
 public:
  struct MeshMaterialPair {
    Mesh* mesh;
    Material* material;
  };
  Shader* shader;
  std::vector<MeshMaterialPair> meshes;

  Model(std::vector<MeshMaterialPair> meshes_, glm::vec3 pos_ = glm::vec3(0.0f))
      : meshes(meshes_), shader(nullptr), Node() {
    position = pos_;
  };
  Model(Mesh* mesh, Material* material, glm::vec3 pos_ = glm::vec3(0.0f))
      : meshes({{mesh, material}}), shader(nullptr), Node() {
    position = pos_;
  };

  Node::NodeType getNodeType() override { return Node::NodeType::Model; };
};

#endif /* Model_hpp */
