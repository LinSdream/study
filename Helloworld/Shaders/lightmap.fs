#version 330 core

struct Material
{
    //漫反射贴图
    sampler2D diffuse;
    //镜面高光贴图
    sampler2D specular;
    //放射光贴图（PS：是一个储存了每个片段的发光值(Emission Value)的贴图。发光值是一个包含（假设）光源的物体发光(Emit)时可能显现的颜色，这样的话物体就能够忽略光照条件进行发光(Glow)）
    sampler2D emission;
    //反光度
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
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec2 emissionOffset;
uniform Material material;
uniform Light light;

void main()
{
    
    //环境光照
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
    
    //漫反射光照
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));

    //镜面光照
    vec3 viewDir = normalize(viewPos - FragPos);//计算视线方向向量
    vec3 reflectDir = reflect(-lightDir,norm);//计算沿法线的反射向量
    float spec = pow(max((dot(viewDir,reflectDir)),0.0),material.shininess);//这里计算的反光度，一个反光度值越高，反射光的能力越强，散射越少，高光点越小，取值为2的幂次方
    vec3 specular = vec3(texture(material.specular,TexCoords)) * spec * light.specular;
    //lighting_maps练习2：反转镜面高光
    //vec3 specular = (vec3(1.0) - vec3(texture(material.specular,TexCoords))) * spec * light.specular;
    vec3 emission = vec3(texture(material.emission,TexCoords + emissionOffset)).rgb;
    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}