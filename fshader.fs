#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform vec3 lightPos;
uniform vec3 lightPos2;
uniform vec3 lightPos3;
uniform vec3 lightPos4;
uniform vec3 lightPos5;
uniform vec3 lightPos6;
uniform vec3 lightPos7;

uniform vec3 lightColor;
uniform vec3 lightColor2;
uniform vec3 lightColor3;
uniform vec3 lightColor4;
uniform vec3 lightColor5;
uniform vec3 lightColor6;
uniform vec3 lightColor7;

void main()
{

    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 lightDir2 = normalize(lightPos2 - FragPos);  
    float diff2 = max(dot(norm, lightDir2), 0.0);
    vec3 diffuse2 = diff2 * lightColor2;

    vec3 lightDir3 = normalize(lightPos3 - FragPos);  
    float diff3 = max(dot(norm, lightDir3), 0.0);
    vec3 diffuse3 = diff3 * lightColor3;

   vec3 lightDir4 = normalize(lightPos4 - FragPos);  
    float diff4 = max(dot(norm, lightDir4), 0.0);
    vec3 diffuse4 = diff4 * lightColor4;

   vec3 lightDir5 = normalize(lightPos5 - FragPos);  
    float diff5 = max(dot(norm, lightDir5), 0.0);
    vec3 diffuse5 = diff5 * lightColor5;

   vec3 lightDir6 = normalize(lightPos6 - FragPos);  
    float diff6 = max(dot(norm, lightDir6), 0.0);
    vec3 diffuse6 = diff6 * lightColor6;

   vec3 lightDir7 = normalize(lightPos7 - FragPos);  
    float diff7 = max(dot(norm, lightDir7), 0.0);
    vec3 diffuse7 = diff7 * lightColor7;
    

    FragColor = texture(ourTexture, texCoord) * vec4(diffuse + diffuse2 + diffuse3 + diffuse4 + diffuse5 + diffuse6 + diffuse7, 1.0);

}