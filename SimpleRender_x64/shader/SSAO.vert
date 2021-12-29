#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;


out vec2 TexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(position, 1.0f);
    gl_Position = projection * view * worldPos;

    TexCoords = gl_Position.xy/gl_Position.w/2.0+0.5;
}