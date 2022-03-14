#version 330 core

out vec4 FragColor;

in vec3 ourColor;// 在OpenGL程序代码中设定这个变量
in vec2 texCoord1;
in vec2 texCoord2;
uniform vec2 towards;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float visibility;

void main()
{
    vec2 temp;
    if(towards.x == 0 || towards.y == 0) 
        temp = vec2(1.0,1.0);
    else
        temp = towards;
    FragColor = mix(texture(ourTexture1,texCoord1),texture(ourTexture2,temp * texCoord2),visibility);// * ourColor;
    //FragColor = texture(ourTexture1,texCoord1) * (1.0-visibility) ;// texture(ourTexture2,texCoord2)* visibility;
    //FragColor = texture(ourTexture2,texCoord2)* visibility;
    //FragColor = texture(ourTexture1,texCoord1) * texture(ourTexture2,temp * texCoord2);// * ourColor;
}