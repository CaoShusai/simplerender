#version 430 core
out vec4 FragColor;  
in vec2 TexCoords;
in vec3 Normal;
uniform vec3 emissive;

void main()
{

    FragColor = vec4(emissive,1.0);
}