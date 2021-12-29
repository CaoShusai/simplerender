#version 430 core
out vec4 FragColor;
//目前只支持单点光源！而且是PCF！！！
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;
uniform vec3 albedo;
uniform samplerCube depthMap;
uniform vec3 lightPositions[10];
uniform vec3 lightColors[10];

uniform vec3 viewPos;

uniform float far_plane;

uniform float metallic;
uniform float roughness;
uniform float ao;

float ShadowCalculation(vec3 fragPos,vec3 lightPos)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
 
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    


	float shadow = 0.0;
    float bias = 0.05;
    float samples = 4.0;
    float offset = 0.1;
    for(float x = -offset; x < offset; x += offset / (samples * 0.5))
    {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5))
        {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r; 
                closestDepth *= far_plane;   // Undo mapping [0;1]
                if(currentDepth - bias > closestDepth);//被遮挡
                else shadow += 1.0;
            }
        }
	}

    shadow /= (samples * samples * samples);
    return shadow;
}


const float PI = 3.14159265359f;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main()
{      
    //vec3  albedo=vec3(0.6,0.2,0.2);
    vec3 N = normalize(fs_in.Normal);
    vec3 V = normalize(viewPos - fs_in.FragPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
	
    // reflectance equation
    vec3 Lo = vec3(0.0);
	vec3 radiance;
	float distance;
	float shadow=0.0f;
	
    for(int i = 0; i < 1; i++)
    {
	    shadow+=ShadowCalculation(fs_in.FragPos,lightPositions[i]);
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - fs_in.FragPos);
        vec3 H = normalize(V + L);
        distance    = length(lightPositions[i] -fs_in.FragPos);
        float attenuation = 1.0 / (distance * distance);
        radiance     = lightColors[i] * attenuation;        

        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= (1.0 - metallic);  

        vec3 nominator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular     = nominator / denominator;

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);              
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }
	clamp(shadow,0.0f,1.0f);
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo*shadow;//shadow为1为无shadow，shadow为0则全遮挡

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
	FragColor=vec4(color,1.0f);
	//FragColor=vec4(vec3(shadow),1.0f);
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