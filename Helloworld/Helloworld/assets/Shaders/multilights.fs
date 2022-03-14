#version 330 core
#define NR_POINT_LIGHTS 4

struct DirLight
{
    vec3 direction;
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;

    float k_constant;
    float k_linear;
    float k_quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    float innerCutOff;
    float outerCutOff;
    
    vec3 ambient;
    sampler2D diffuse;
    vec3 specular;

    float linear;
    float constant;
    float quadratic;

};

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;

    float shininess;
};

out vec4 FragColor;

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform vec2 emissionOffset;

uniform Material material;
uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 viewDir);
vec3 CalcSpotlight(SpotLight light,vec3 nromal,vec3 viewDir);

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(normal);
    vec3 result = CalcDirLight(dirLight,norm,viewDir);
    for(int i=0;i<NR_POINT_LIGHTS;i++)
    {
        result += CalcPointLight(pointLights[i],norm,viewDir);
    }

    result += CalcSpotlight(spotLight,norm,viewDir);
    FragColor = vec4(result,1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{

    vec3 result;
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;
    float diff = max(dot(lightDir,normal),0.0);
    vec3 diffuse = diff * light.diffuse * texture(material.diffuse,TexCoords).rgb;

    vec3 reflectDir =reflect(-lightDir,normal);
    float spec = pow(max(dot(lightDir,normal),0.0),material.shininess);
    vec3 specular = spec * texture(material.specular,TexCoords).rgb * light.specular;

    vec3 emission = vec3(texture(material.emission,TexCoords + emissionOffset)).rgb;

    result = ambient + diffuse + specular;// + emission;

    return result;
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 viewDir)
{
    vec3 result;
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;

    float diff = max(dot(lightDir,normal),0.0);
    vec3 diffuse = diff * texture(material.diffuse,TexCoords).rgb * light.diffuse;

    vec3 reflectDir = reflect(-lightDir,normal);
    
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = spec * texture(material.specular,TexCoords).rgb * light.specular;

    vec3 emission = texture(material.emission, TexCoords + emissionOffset).rgb;
    
    float distance = length(light.position - FragPos);
    float attenuation = 1.0/(light.k_constant + light.k_linear * distance + light.k_quadratic * distance * distance);
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    emission *= attenuation;

    result  = ambient + diffuse + specular ;//+ emission;

    return result;
}

vec3 CalcSpotlight(SpotLight light,vec3 nromal,vec3 viewDir)
{
    vec3 result;
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir,normalize(-light.direction));
    float  epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon,0.0,1.0);
    
    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;

    float diff = max(dot(lightDir,normal),0.0);
    vec3 diffuse = diff * texture(material.diffuse,TexCoords).rgb + texture(light.diffuse,TexCoords).rgb * diff;

    vec3 reflectDir = reflect(-lightDir,normal);
    
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = spec * texture(material.specular,TexCoords).rgb * light.specular;

    vec3 emission = texture(material.emission, TexCoords + emissionOffset).rgb;
    
    float distance = length(light.position - FragPos);
    float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * distance * distance);
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    emission *= attenuation;

    ambient *= intensity;
    diffuse *= intensity;
    specular *=intensity;
    emission *= intensity;

    result = ambient + diffuse + specular ;//+ emission;

    return result;
}