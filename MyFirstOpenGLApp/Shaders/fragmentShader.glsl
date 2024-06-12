#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct DirectionalLight {
    vec3 lightColor;
    vec3 direction;
    float intensity;
};

struct PointLight {
    vec3 lightColor;
    vec3 pos;
    float constant;
    float linear;
    float quadratic;
    float intensity;
};

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform bool useDiffuseMap;
uniform bool useSpecularMap;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;
uniform vec3 viewerPos;
#define MAX_POINT_LIGHTS 4
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform DirectionalLight dirLight;
uniform float hasDirLight;
uniform int pointLightsCount;

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 diffuseMaterialColor, vec3 specularMaterialColor) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient  = light.lightColor * 0.1 * diffuseMaterialColor;
    vec3 diffuse  = light.lightColor  * diff * diffuseMaterialColor;
    vec3 specular = light.lightColor * specularMaterialColor * spec*0.5;
    return ((ambient + diffuse + specular)*light.intensity*hasDirLight);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 diffuseMaterialColor, vec3 specularMaterialColor) {
    float distance    = length(light.pos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));
    vec3 lightDir = normalize(light.pos - FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient  = light.lightColor * 0.1 * diffuseMaterialColor*attenuation;
    vec3 diffuse  = light.lightColor  * diff * diffuseMaterialColor*attenuation;
    vec3 specular = light.lightColor * specularMaterialColor * spec*0.5*attenuation;
    return (ambient + diffuse + specular)*light.intensity;
}

void main()
{
    vec3 norm = Normal;
    vec3 viewDir = normalize(viewerPos - FragPos);
    
    vec3 fragmentDiffuseColor = diffuseColor;
    if (useDiffuseMap) {
        fragmentDiffuseColor = texture(diffuseMap, TexCoord).xyz;
    }
    
    vec3 fragmentSpecularColor = specularColor;
    if (useSpecularMap) {
        fragmentSpecularColor = texture(specularMap, TexCoord).xyz;
    }
    
    vec3 result = calcDirLight(dirLight, norm, viewDir, fragmentDiffuseColor, fragmentSpecularColor);
    for (int i = 0; i < pointLightsCount; i++) {
        result += calcPointLight(pointLights[i], norm, viewDir, fragmentDiffuseColor, fragmentSpecularColor);
    }
    
    FragColor = vec4(result, 1.0);
}
