#version 330 core

out vec4 FragColor;

in vec3 ourColor;// 在OpenGL程序代码中设定这个变量
in vec2 texCoord;
uniform vec2 towards;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    vec2 temp;
    if(towards.x == 0 || towards.y == 0) 
        temp = vec2(1.0,1.0);
    else
        temp = towards;
    FragColor = mix(texture(ourTexture1,texCoord),texture(ourTexture2,temp * texCoord),0.2);// * ourColor;
}