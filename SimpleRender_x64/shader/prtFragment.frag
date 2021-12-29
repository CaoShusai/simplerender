#version 430 core
out vec4 FragColor;  
in vec3 ourColor;
in vec2 TexCoords;
in vec3 Normal;
void main()
{
    //vec3 Color;
    //Color = ourColor / (ourColor + vec3(1.0));
    //Color = pow(Color, vec3(1.0/2.2));
    FragColor = vec4(ourColor, 1.0);
}