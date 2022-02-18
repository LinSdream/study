#version 330 core

out vec4 FragColor;

struct Light
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float k_constant;
    float k_linear;
    float k_quadratic;

    float cutoff;
};

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform vec2 emissionOffset;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);

    float theta = dot(lightDir,normalize(-light.direction));
    if(theta <= light.cutoff)
    {
        FragColor = vec4(light.ambient * texture(material.diffuse,TexCoords).rgb,1.0);
        return;
    }

    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;

    vec3 norm = normalize(normal);
    float diff = max(dot(lightDir ,norm),0.0);
    vec3 diffuse = diff * texture(material.diffuse,TexCoords).rgb * light.diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = spec * texture(material.specular,TexCoords).rgb * light.specular;

    vec3 emission = vec3(texture(material.emission,TexCoords + emissionOffset)).rgb;

    //光的衰减计算,衰减值将用于乘上光的强度向量，以此达到近亮远暗
    //公式：Fatt = 1.0 / (Kc + Kl * d + Kq * d^2)
    //计算光到顶点的长度
    float dist = length(light.position - FragPos);
    float attenuation = 1.0/(light.k_constant + light.k_linear * dist + light.k_quadratic * dist * dist);
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    emission *= attenuation;

    vec3 result = ambient + diffuse + specular + emission;

    FragColor = vec4(result,1.0);
}