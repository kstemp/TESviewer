#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture;

void main()
{

    FragColor = texture(ourTexture, texCoord);

 

}