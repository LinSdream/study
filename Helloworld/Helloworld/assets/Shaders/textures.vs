#version 330 core
layout (location = 0) in vec3 aPos;//λ�ñ���������λ��ֵΪ0
layout (location = 1) in vec3 aColor;//��ɫ����������λ��ֵΪ1
layout (location = 2) in vec2 aTexCoord;//��������
layout (location = 3) in vec2 aTexCoord2;
uniform vec3 aPosXYZ;
out vec3 ourColor;//ΪƬ����ɫ��ָ��һ����ɫ����
uniform bool useOffset;//�Ƿ�ʹ���Զ���λ��
uniform mat4 transform;
out vec2 texCoord1;
out vec2 texCoord2;

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
    gl_Position = transform*temp ;
    ourColor = aColor;
    texCoord1 = vec2(aTexCoord.x,aTexCoord.y);
    texCoord2 = vec2(aTexCoord2.x,aTexCoord2.y);
}