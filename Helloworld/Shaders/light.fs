#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec3 normal;
in vec3 FragPos;
in vec3 CubeColor;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 objectColor;

uniform Material material;
uniform Light light;

void main()
{
    
    //环境光照
    vec3 ambient = material.ambient * light.ambient;
    
    //漫反射光照
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    //镜面光照
    vec3 viewDir = normalize(viewPos - FragPos);//计算视线方向向量
    vec3 reflectDir = reflect(-lightDir,norm);//计算沿法线的反射向量
    float spec = pow(max((dot(viewDir,reflectDir)),0.0),material.shininess);//这里计算的反光度，一个反光度值越高，反射光的能力越强，散射越少，高光点越小，取值为2的幂次方
    vec3 specular = (material.specular * spec) * light.specular;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}