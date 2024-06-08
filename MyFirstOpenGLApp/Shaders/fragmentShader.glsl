#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture0;
uniform float textureMix;
uniform vec3 aColor;
uniform vec3 viewerPos;

void main()
{
    vec3 lightPos = vec3(0.0, 2.0, 2.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    float ambientStrength = 0.1;
    float specularStrength = 1;
    
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 viewDir = normalize(viewerPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = ambient+diffuse+specular;
    
    FragColor = mix(
                    vec4(aColor, 1),
                    texture(texture0, TexCoord),
                    textureMix
                    )*vec4(result, 1.0);
}
