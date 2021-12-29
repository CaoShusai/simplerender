#version 430 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
	//FragColor =vec4(0.6f,0.2f,0.2f,1.0f);
}