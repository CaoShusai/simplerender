#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

//uniform sampler2D depthMap;
uniform sampler2D texture1;
uniform float near_plane;
uniform float far_plane;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{             
    vec4 Value = texture(texture1, TexCoords);
    // FragColor = vec4(LinearizeDepth(depthValue) / far_plane,0.0,0.0, 1.0); // perspective
    FragColor = vec4(Value.r,Value.r,Value.r,1.0); // orthographic
	//FragColor = vec4(near_plane*10,far_plane/50,0,0);
}