//
//  Node.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 16.6.2024.
//

#ifndef Node_hpp
#define Node_hpp

#include <vector>

struct Node {
    enum class NodeType {
        Scene,
        Model,
        Light
    };
    Node* parentNode;
    std::vector<Node*> children;
    
    Node(): parentNode(nullptr) {};
    Node(Node* parent): parentNode(parent) {};
    virtual NodeType getNodeType() {return NodeType::Scene;};
};

#endif /* Node_hpp */
