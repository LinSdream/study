#version 330 core

in vec3 normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float specularStrength; //镜面强度

void main()
{
    //环境光照
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    //漫反射光照
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff * lightColor;

    //镜面光照
    vec3 viewDir = normalize(viewPos - FragPos);//计算视线方向向量
    vec3 reflectDir = reflect(-lightDir,norm);//计算沿法线的反射向量
    float spec = pow(max((dot(viewDir,reflectDir)),0.0),32);//这里计算的反光度，一个反光度值越高，反射光的能力越强，散射越少，高光点越小，取值为2的幂次方
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}