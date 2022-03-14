#version 330 core
layout (location = 0) in vec3 aPos;//λ�ñ���������λ��ֵΪ0

layout (location = 1) in vec3 aColor;//��ɫ����������λ��ֵΪ1
uniform vec3 aPosXYZ;
out vec4 ourColor;//ΪƬ����ɫ��ָ��һ����ɫ����
uniform bool useOffset;//�Ƿ�ʹ���Զ���λ��

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
        temp = vec4(aPos,1.0);//��һ��vec3��Ϊvec4�Ĺ������Ĳ���
    }
    gl_Position = temp;
    ourColor = vec4(aColor,1.0);
}