#version 330 core
layout (location = 0) in vec3 aPos;//位置变量的属性位置值为0

layout (location = 1) in vec3 aColor;//颜色变量的属性位置值为1
uniform vec3 aPosXYZ;
out vec4 ourColor;//为片段着色器指定一个颜色输入
uniform bool useOffset;//是否使用自定义位置

void main()
{

    vec4 temp=vec4(0.0,0.0,0.0,0.0);
    if(useOffset)
    {
        temp = vec4(aPos+aPosXYZ,1.0);
        temp.x = temp.x>1.0?1.0:temp.x;
        temp.x = temp.x<-1.0?-1.0:temp.x;

        temp.y = temp.y>1.0?1.0:temp.y;
        temp.y = temp.y<-1.0?-1.0:temp.y;
        
        temp.z = temp.z>1.0?1.0:temp.z;
        temp.z = temp.z<-1.0?-1.0:temp.z;
    }
    else
    {
        temp = vec4(aPos,1.0);//将一个vec3作为vec4的构造器的参数
    }
    gl_Position = temp;
    ourColor = vec4(aColor,1.0);
}