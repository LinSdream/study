#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec2 aTexCoord2;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 texCoord1;
out vec2 texCoord2;


void main()
{
	gl_Position = projection * view * model * vec4(aPos,1.0);
	texCoord1 = vec2(aTexCoord.x,aTexCoord.y);
    texCoord2 = vec2(aTexCoord2.x,aTexCoord2.y);

	texCoord1 = aTexCoord;
	texCoord2 = aTexCoord2;
}