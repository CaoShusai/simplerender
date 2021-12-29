#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
in vec3 Normal;
in vec3 FragPos;
uniform sampler2D texture_diffuse1;
uniform float shininess;
uniform vec3 viewPos;
uniform int PointLightNum;
uniform PointLight pointLights[10];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result=vec3(0.0,0.0,0.0);
	for(int i = 0; i < PointLightNum; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);   
    FragColor = texture(texture_diffuse1,TexCoord);
	//FragColor = vec4(1.0,0.5,0.5,1.0);
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(0.5f,0.2f,0.2f);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}