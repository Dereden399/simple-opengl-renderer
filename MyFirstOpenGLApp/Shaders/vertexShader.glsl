#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNorm;

out vec2 TexCoord;

uniform mat4 projectionView;
uniform mat4 model;

void main()
{
    gl_Position = projectionView * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
