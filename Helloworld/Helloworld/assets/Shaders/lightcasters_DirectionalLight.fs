#version 330 core
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform vec2 emissionOffset;

void main()
{
    //平行光的方向一般定义为从光源向外的方向，因此这里需要取反
    vec3 lightDir = normalize(-light.direction);
    
    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;

    vec3 norm = normalize(normal);
    float diff = max(dot(lightDir ,norm),0.0);
    vec3 diffuse = diff * vec3(texture(material.diffuse,TexCoords)) * light.diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = spec * texture(material.specular,TexCoords).rgb * light.specular;

    vec3 emission = vec3(texture(material.emission,TexCoords + emissionOffset)).rgb;

    vec3 result = ambient + diffuse + specular + emission;

    FragColor = vec4(result,1.0);    
}