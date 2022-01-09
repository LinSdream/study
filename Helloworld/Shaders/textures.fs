#version 330 core

out vec4 FragColor;

in vec4 ourColor;// 在OpenGL程序代码中设定这个变量
in vec2 texCoord;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    FragColor = mix(texture(ourTexture1,texCoord),texture(ourTexture2,texCoord),0.2);// * ourColor;
}