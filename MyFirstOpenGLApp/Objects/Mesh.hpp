//
//  Mesh.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 7.6.2024.
//

#ifndef Mesh_hpp
#define Mesh_hpp
#include <vector>
#include <string>

#include "Vertex.hpp"

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> localIndices;
    std::string name;
    unsigned int size;
    unsigned int globalStartIndex;
    
    Mesh(std::vector<Vertex> vertices_, std::vector<unsigned int> localIndices_, std::string name_)
    : vertices(vertices_), localIndices(localIndices_), name(name_) {
        globalStartIndex = 0;
        size = (unsigned int)localIndices_.size();
    };
    bool operator==(const Mesh& second) {return this->name == second.name;};
};

class CubeMesh: public Mesh {
public:
    CubeMesh() : Mesh(getCubeVertices(), getCubeIndices(), "cube") {};
private:
    std::vector<Vertex> getCubeVertices() {
        return std::vector<Vertex>{
            // Back face
            Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f),
            Vertex( 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f),
            Vertex( 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f),
            Vertex(-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f),

            // Front face
            Vertex(-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f),
            Vertex( 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f),
            Vertex( 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f),
            Vertex(-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f),

            // Left face
            Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f),
            Vertex(-0.5f,  0.5f, -0.5f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f),
            Vertex(-0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  -1.0f, 0.0f, 0.0f),
            Vertex(-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f),

            // Right face
            Vertex( 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f),
            Vertex( 0.5f,  0.5f, -0.5f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f),
            Vertex( 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
            Vertex( 0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f),

            // Bottom face
            Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f),
            Vertex( 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0f),
            Vertex( 0.5f, -0.5f,  0.5f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f),
            Vertex(-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f),

            // Top face
            Vertex(-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f),
            Vertex( 0.5f,  0.5f, -0.5f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f),
            Vertex( 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f),
            Vertex(-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f)
        };
    };
    std::vector<unsigned int> getCubeIndices() {
        return
        std::vector<unsigned int>{
            // Back face
            0, 1, 2, 2, 3, 0,
            // Front face
            4, 5, 6, 6, 7, 4,
            // Left face
            8, 9, 10, 10, 11, 8,
            // Right face
            12, 13, 14, 14, 15, 12,
            // Bottom face
            16, 17, 18, 18, 19, 16,
            // Top face
            20, 21, 22, 22, 23, 20};
    };
};

#endif /* Mesh_hpp */
