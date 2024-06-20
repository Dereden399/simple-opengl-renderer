#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out mat3 TBN;

uniform mat4 projectionView;
uniform mat4 model;
uniform mat4 normalsModel;
uniform float textureScale;

void main()
{
    gl_Position = projectionView * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord*textureScale;
    FragPos = vec3(model*vec4(aPos,1.0));
    Normal = normalize((normalsModel*vec4(aNorm, 0.0)).xyz);
    vec3 T = normalize(vec3(normalsModel * vec4(aTangent,   0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, Normal) * Normal);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(Normal, T);
    TBN = mat3(T, B, Normal);
}
