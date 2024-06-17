#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 projectionView;
uniform mat4 model;
uniform mat4 normalsModel;

void main()
{
    gl_Position = projectionView * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    FragPos = vec3(model*vec4(aPos,1.0));
    Normal = normalize((normalsModel*vec4(aNorm, 0.0)).xyz);
}
