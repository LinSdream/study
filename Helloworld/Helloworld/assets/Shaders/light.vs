#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aCubeColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec3 normal;
out vec3 FragPos;
out vec3 CubeColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos,1.0);

    //法线矩阵，在model发生缩放变换的时候，对于非等比缩放，法线将不会垂直于对应的表明，为了修复该行为，法线需要通过线性代数来移除法线的缩放，这种叫做法线矩阵
    //document:
    //OpenGL源教程：<https://learnopengl-cn.github.io/02%20Lighting/02%20Basic%20Lighting/#_6>
    //原文的证明链接：<http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/>
    //评论区的证明连接:<http://www.songho.ca/opengl/gl_normaltransform.html> 二者区别在于，前者是只采用了左上角3*3的矩阵，而后者是完全的包含齐次w的矩阵
    //PS:没去细看比较，在此备注
    //总之由于计算逆矩阵在着色器中属于开销较大的计算，因此需要丢到CPU中去计算会比较好，这里仅留下计算内容 法线矩阵 G = (M^-1)^T
    //normalMatrix = mat3(transpose(inverse(model)));
    normal = normalMatrix * aNormal;
    CubeColor = aCubeColor;
    FragPos = vec3(model * vec4(aPos,1.0));
}