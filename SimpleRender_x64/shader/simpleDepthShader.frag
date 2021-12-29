#version 430 core
out vec4 FragColor;
void main()
{      
     FragColor = vec4(gl_FragCoord.z,0.0,0.0,1.0);
//	 gl_FragDepth = gl_FragCoord.z;
	// FragColor =
    // gl_FragDepth = gl_FragCoord.z;
}