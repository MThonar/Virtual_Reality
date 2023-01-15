#version 330 core

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPosText;

out vec4 FragColor;

uniform sampler2D textureExpl;

uniform float ambient;
uniform float specularStrength;
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform vec3 viewPos;

void main()
{   
    //Diffuse Light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPosText);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //Ambient Light
    vec3 ambientScene = ambient * lightColor;

    //Specular Light
    vec3 viewDir = normalize(viewPos - FragPosText);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    //Global
    vec3 colorScene = ambientScene + diffuse + specular;
    FragColor = vec4(vec4(0.97,0.90,0.0,1.0));// *(texture(textureNormal, TexCoords)));
}