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

class Model : public Node, public Movable, public Rotatable, public Scalable {
    struct MeshMaterialPair {
        Mesh* mesh;
        Material* material;
    };
public:
    Shader* shader;
    std::vector<MeshMaterialPair> meshes;
    
    Model(std::vector<MeshMaterialPair> meshes_, glm::vec3 pos_ = glm::vec3(0.0f)): meshes(meshes_), shader(nullptr), Movable(pos_), Node() {};
    Model(Mesh* mesh, Material* material, glm::vec3 pos_ = glm::vec3(0.0f)): meshes({{mesh, material}}), shader(nullptr), Movable(pos_), Node() {};
    
    glm::mat4 getModelMatrix();
    
    Node::NodeType getNodeType() override {return Node::NodeType::Model;};
};

#endif /* Model_hpp */
