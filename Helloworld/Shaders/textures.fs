#version 330 core

out vec4 FragColor;

in vec4 ourColor;// 在OpenGL程序代码中设定这个变量
in vec2 texCoord;
uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture,texCoord);
}