#version 330 core
out vec4 FragColor_text;
  
in vec3 ourColor_text;
in vec2 TexCoord_text;
in vec3 Normal;  
in vec3 FragPos_text;
uniform sampler2D ourTexture_text;
uniform sampler2D texture_text2;

uniform float ambient;
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform vec3 viewPos;

void main()
{   
     

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos_text);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 ambientScene = ambient * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos_text);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    vec3 colorscene = ambientScene + diffuse + specular;
    FragColor_text = vec4(vec4(colorscene,1.0) *(mix(texture(ourTexture_text, TexCoord_text), texture(texture_text2, TexCoord_text), 0.5)));
}