#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 1) in vec3 aNormal;

out vec2 TexCoords;
//out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec3 WorldPos;
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal=mat3(transpose(inverse(model)))*aNormal;
	
	//ourColor=vec3(0.5,0.2,0.2);
    gl_Position =  projection * view * vec4(WorldPos, 1.0);
	
}