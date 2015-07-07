#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
	
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 3

//in vec2 TextCoords;
in vec3 PositionNormal;
in vec3 FragPos;

uniform vec3 cameraPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform float numberoflights;

out vec4 color;

// Function prototypes
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
	//color = textureProj(material.diffuse,  FragPos);
	
	// Properties
    vec3 norm = normalize(PositionNormal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    
    // Phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
	
	for(int i = 0; i < numberoflights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
	
	color = vec4(result, 1.0f);

	
	/*float depthValue = texture(depthMap, TexCoords).r;
    color = vec4(vec3(depthValue), 1.0);*/
	
	//vec3 color = vec3(texture(diffuseTexture, fs_in.TexCoords));
	/*vec3 color = vec3(textureProj(material.diffuse, FragPos));
    vec3 normal = normalize(PositionNormal);
    vec3 lightColor = vec3(1.0f);

    vec3 ambient = 0.0f * color;

    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // Specular
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // Calculate shadow
    //float shadow = ShadowCalculation(fs_in.FragPosLightSpace);       
    vec3 lighting = (ambient * (diffuse + specular)) * color;    
    
    fragColor = vec4(lighting, 1.0f);*/
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Combine results
    vec3 ambient = light.ambient * vec3(textureProj(material.diffuse, FragPos));
    vec3 diffuse = light.diffuse * diff * vec3(textureProj(material.diffuse, FragPos));
    vec3 specular = max(light.specular * spec * vec3(textureProj(material.specular, FragPos)),0.0f);
    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient = light.ambient * vec3(textureProj(material.diffuse, FragPos));
    vec3 diffuse = light.diffuse * diff * vec3(textureProj(material.diffuse, FragPos));
    vec3 specular = max(light.specular * spec * vec3(textureProj(material.specular, FragPos)),0.0f);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}