#version 430 core
#define kernelSize 64
#define radius 1.0
out float FragColor;
in vec2 TexCoords;
const float NEAR = 0.1; // ͶӰ����Ľ�ƽ��
const float FAR = 50.0f; // ͶӰ�����Զƽ��
uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
// ��Ļ��ƽ����������������Ļ�ֱ��ʳ���������С��ֵ������
const vec2 noiseScale = vec2(1440/4.0, 960.0/4.0); 
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // �ص�NDC
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));    
}
void main()
{
    vec4 fragPos = texture(gPositionDepth, TexCoords);
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz;

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;
	
    for(int i = 0; i < kernelSize; ++i)
    {
    // ��ȡ����λ��
	    vec3 sample1;
        sample1 = TBN * samples[i]; // ����->�۲�ռ�
        sample1 = fragPos.xyz + sample1 * radius; 
		vec4 offset = vec4(sample1, 1.0);
        offset = projection * offset; // �۲�->�ü��ռ�
		

        offset.xyz /= offset.w; // ͸�ӻ���
        offset.xyz = offset.xyz * 0.5 + 0.5; // �任��0.0 - 1.0��ֵ��
		float sampleDepth = texture(gPositionDepth, offset.xy).w;
		
		
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		//float rangeCheck=0.0;
		//if(abs(fragPos.z - sampleDepth)<radius) rangeCheck=1.0f;
		occlusion += (sampleDepth >= sample1.z ? 1.0 : 0.0) * rangeCheck;//1.0���ڵ�
    }
	occlusion = 1.0 - (occlusion / kernelSize);//
    FragColor = occlusion;  
	
}