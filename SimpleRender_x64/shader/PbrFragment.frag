#version 430 core
#define PCF_NUM_SAMPLES 16
#define BLOCKER_SEARCH_NUM_SAMPLES 16 
#define LIGHT_WORLD_SIZE .5
#define LIGHT_FRUSTUM_WIDTH 3.75 
#define LIGHT_SIZE_UV (LIGHT_WORLD_SIZE / LIGHT_FRUSTUM_WIDTH) 
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
uniform float near_plane;
uniform float far_plane;
// material parameters
uniform vec3  albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform mat4 lightSpaceMatrix;
// lights
uniform vec3 lightPositions[10];
uniform vec3 lightColors[10];

uniform vec3 camPos;
uniform int numBlockerSearchSamples = 16;
uniform float NEAR=0.1f;
uniform sampler2D depthMap;


uniform vec2 poissonDisk[16] = {
 vec2( -0.94201624, -0.39906216 ),
 vec2( 0.94558609, -0.76890725 ),
 vec2( -0.094184101, -0.92938870 ),
 vec2( 0.34495938, 0.29387760 ),
 vec2( -0.91588581, 0.45771432 ),
 vec2( -0.81544232, -0.87912464 ),
 vec2( -0.38277543, 0.27676845 ),
 vec2( 0.97484398, 0.75648379 ),
 vec2( 0.44323325, -0.97511554 ),
 vec2( 0.53742981, -0.47373420 ),
 vec2( -0.26496911, -0.41893023 ),
 vec2( 0.79197514, 0.19090188 ),
 vec2( -0.24188840, 0.99706507 ),
 vec2( -0.81409955, 0.91437590 ),
 vec2( 0.19984126, 0.78641367 ),
 vec2( 0.14383161, -0.14100790 )
};

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}


vec2 RandomDirection(sampler1D distribution, float u)
{
   return texture(distribution, u).xy * 2 - vec2(1);
}

float SearchWidth(float uvLightSize, float receiverDistance)
{
	return uvLightSize * (receiverDistance - NEAR) / receiverDistance;
}

vec3 processCoords()
{
    vec4 projectCoords=lightSpaceMatrix*vec4(WorldPos,1.0f);
	vec3  projCoords = projectCoords.xyz/projectCoords.w;
	return projCoords*0.5+0.5;
}

float findBlockDepth(float uvLightSize,float receiverDistance,vec2 uv)
{
    float searchwidth = SearchWidth(uvLightSize,receiverDistance);
	float blockerSum = 0.0;
	int numBlockers = 0;
	for( int i = 0; i < BLOCKER_SEARCH_NUM_SAMPLES; ++i )
    {
	    vec2 offset = poissonDisk[i] * searchwidth;
        float shadowMapDepth = texture(depthMap,uv+offset).r;
        if ( shadowMapDepth < receiverDistance-0.002f ) 
		{
            blockerSum += shadowMapDepth;
            numBlockers++;
         }
     }
	 if(numBlockers==0) return -1;
     return blockerSum / numBlockers;
	//return  texture(depthMap,uv).r;
}

float PCF_Filter(vec2 uv,float receiverDistance,float filterRadiusUV)
{
    float sum=0.0f;
    for ( int i = 0; i < PCF_NUM_SAMPLES; ++i )
    {
        vec2 offset = poissonDisk[i] * filterRadiusUV;
		float z=texture(depthMap,uv+offset).r;
        sum += (z<(receiverDistance-0.002f))? 0:1;//遮挡了加0，系数小，最后的值小
     }
    return sum / PCF_NUM_SAMPLES;
}

float pcss_shadow(float uvLightSize)
{
    vec3 tmpCoords = processCoords();
    vec2 uv = tmpCoords.xy;
	float receiverDistance = tmpCoords.z;
	float avgBlockerDepth = -1;
	avgBlockerDepth = findBlockDepth(uvLightSize,receiverDistance,uv);

	//return texture(depthMap,uv).r;

	if(avgBlockerDepth == -1)
	    return 1.0f;//没有阴影返回系数1
	float filterRadiusUV=(receiverDistance-avgBlockerDepth)*uvLightSize*NEAR/avgBlockerDepth/receiverDistance;
	return PCF_Filter(uv,receiverDistance,filterRadiusUV);
}



void main()
{       
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
	vec3 radiance;
	float distance;
    for(int i = 0; i < 1; i++) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        distance    = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        radiance     = lightColors[i] * attenuation;        

        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;     

        vec3 nominator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
        vec3 specular     = nominator / denominator;

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    }   
	float shadow=pcss_shadow(LIGHT_SIZE_UV);
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo*shadow;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
	FragColor=vec4(color,1.0f);
    //FragColor = vec4(vec3(shadow,shadow,shadow), 1.0);
	
}  

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}