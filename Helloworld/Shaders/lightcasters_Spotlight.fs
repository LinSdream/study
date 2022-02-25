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

    float innterCutOff;
    float outerCutOff;
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

    //光的衰减计算,衰减值将用于乘上光的强度向量，以此达到近亮远暗
    //公式：Fatt = 1.0 / (Kc + Kl * d + Kq * d^2)
    //计算光到顶点的长度
    float dist = length(light.position - FragPos);
    float attenuation = 1.0/(light.k_constant + light.k_linear * dist + light.k_quadratic * dist * dist);
    
    float theta = dot(lightDir,normalize(-light.direction));

    // doc:<https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/#_9>
    //平滑边缘，实际上就是在两个光锥，内锥和外锥，内外锥之间的余弦值进行差值。
    //公式： I = (θ-γ)/ε;ε = (φ-γ)   ε:内圆锥(φ) 与外圆锥(γ)的余弦值差值 。θ：内圆锥余弦值，γ：外圆锥余弦值
    float epsilon = light.innterCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon , 0.0 ,1.0);

    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;

    vec3 norm = normalize(normal);
    float diff = max(dot(lightDir ,norm),0.0);
    vec3 diffuse = diff * texture(material.diffuse,TexCoords).rgb * light.diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = spec * texture(material.specular,TexCoords).rgb * light.specular;

    //vec3 emission = vec3(texture(material.emission,TexCoords + emissionOffset)).rgb;
  
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    //emission *= attenuation;

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    vec3 result = ambient + diffuse + specular ;//+ emission;

    FragColor = vec4(result,1.0);
}