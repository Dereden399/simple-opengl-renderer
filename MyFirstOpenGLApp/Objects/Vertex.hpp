//
//  Vertex.hpp
//  MyFirstOpenGLApp
//
//  Created by Denis Kuznetsov on 6.6.2024.
//

#ifndef Vertex_hpp
#define Vertex_hpp

struct Vertex {
    float x,y,z;
    float u,w;
    float nx,ny,nz;
    
    Vertex(float x_, float y_, float z_) : x(x_), y(y_), z(z_), u(0), w(0), nx(0), ny(0), nz(0) {};
    Vertex(float x_, float y_, float z_, float u_, float w_) : x(x_), y(y_), z(z_), u(u_), w(w_), nx(0), ny(0), nz(0) {};
};

#endif /* Vertex_hpp */
