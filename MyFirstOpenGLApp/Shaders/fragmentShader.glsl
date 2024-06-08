#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture0;
uniform float textureMix;
uniform vec3 aColor;

void main()
{
    FragColor = mix(
                    vec4(aColor, 1),
                    texture(texture0, TexCoord),
                    textureMix
                    );
}
