//
//  Node.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 16.6.2024.
//

#ifndef Node_hpp
#define Node_hpp

#include <vector>
#include <glm/glm.hpp>
#include "Utils.hpp"

struct Node : public Transform {
  enum class NodeType { Scene, Model, Light };
  Node* parentNode;
  std::vector<Node*> children;
  glm::mat4 toParentTransform;

  Node()
      : parentNode(nullptr), toParentTransform(glm::mat4(1.0f)), Transform(){};
  Node(Node* parent)
      : parentNode(parent), toParentTransform(glm::mat4(1.0f)), Transform(){};
  void addChild(Node* n) {
    children.push_back(n);
    n->parentNode = this;
  };

  virtual NodeType getNodeType() { return NodeType::Scene; };
};

#endif /* Node_hpp */
