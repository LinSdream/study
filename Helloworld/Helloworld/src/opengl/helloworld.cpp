#include <opengl/helloworld.h>
#include<opengl/helperFun.h>
#include<stb/stb_image.h>
#include<windows/window.h>

        //\          Seeyou           /
        // \                         /
        //  \    Too Hard	        /
        //   ]   not o l		   [    ,'|
        //   ]                     [   /  |
        //   ]___               ___[ ,'   |
        //   ]  ]\             /[  [ |:   |
        //   ]  ] \           / [  [ |:   |
        //   ]  ]  ]         [  [  [ |:   |
        //   ]  ]  ]__     __[  [  [ |:   |
        //   ]  ]  ] ]\ _ /[ [  [  [ |:   |
        //   ]  ]  ] ] (#) [ [  [  [ :===='
        //   ]  ]  ]_].nHn.[_[  [  [
        //   ]  ]  ]  HHHHH. [  [  [
        //   ]  ] /   `HH("N  \ [  [
        //   ]__]/     HHH  "  \[__[
        //   ]         NNN         [
        //   ]         N/"         [
        //   ]         N H         [
        //  /          N            \
        // /           q,            \
        ///                           \

bool LoadImage2D(const uint* textureID, const char* path, GLint wrapST_param, GLint filter_param)
{

	if (path == NULL || std::strlen(path) == 0) return false;

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	uchar* image = stbi_load(path, &width, &height, &nrChannels, 0);
	if (!image)
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(image);
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, *textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapST_param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapST_param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_param);

	GLenum format;
	if (nrChannels == 1) format = GL_RED;
	else if (nrChannels == 3) format = GL_RGB;
	else if (nrChannels == 4) format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image);
	return true;
}

void HelloworldGradientEnvironment::Background(GLFWwindow* window)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

DrawTriangle::DrawTriangle(Shader* shader) :DrawBase(shader)
{
	vao_ = new VAOContext();
	vbo_ = new VBOContext();
}

DrawTriangle::~DrawTriangle()
{
	delete vao_;
	delete vbo_;
}

void DrawTriangle::Init(const void* context)
{
	vao_->Bind();
	vbo_->Bind();

	float vertices[] = {
		//λ��				//��ɫ
		-0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f,// left  
		0.1f, -0.1f, 0.0f, 0.0f, 1.0f, 0.0f, // right 
		0.0f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f,// top   
	};

	//usage:��ʾ���ǻ������ᱻopenGL���ʹ�ã�������������
	//GL_STATIC_DRAW:��ʾ�û��������ᱻ�޸�
	//GL_DYNAMIC_DRAW:��ʾ�û������ᱻ�������޸�
	//GL_STREAM_DRAW:��ʾ�û������ᱻƵ���޸�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//�ο��ĵ�<https://learnopengl-cn.github.io/01%20Getting%20started/05%20Shaders/#_5>
	//����˵���Ǵ��ݽ�ȥ������ֻ��һ��һά�����ݣ���ʵ������£����������ж��غ���ģ����Ծ���Ҫȥָ�����͸������ݵĺ���
	//����λ����Ϣ
	//��ʼλ��Ϊ0��ÿ3������Ϊһ�����ݣ���һ��Ŀ�ʼ�Ǵ�0��ʼ������ƫ��6��float���ȵ�����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//������ɫ��Ϣ
	//��ʼλ��Ϊ0��ÿ3������Ϊһ�����ݣ���һ��Ŀ�ʼ�Ǵ�3��float֮���λ�ã�����ƫ��6��float���ȵ�����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//������ΪĬ��ֵ,���ں�����Draw�����½��а󶨣�������ʵ���Բ��ã�����Ϊ���Ͻ����ﻹ�ǽ���һ�ε�����
	//ͨ����0��������ǰ�Ļ�������ΪĬ��ֵ
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//ͨ����0�������������������ΪĬ��ֵ
	glBindVertexArray(0);
}

void DrawTriangle::Update(const void* context)
{
	shader_->Use();
	vao_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

DrawRectangle::DrawRectangle(Shader* shader):DrawBase(shader)
{
	vao_ = new VAOContext();
	ebo_ = new EBOContext();
	vbo_ = new VBOContext();

	glGenTextures(1, &texture1_);
	glGenTextures(1, &texture2_);

	visibilityValue_ = 0.0f;
}

DrawRectangle::~DrawRectangle() 
{
	glDeleteTextures(1, &texture1_);
	glDeleteTextures(1, &texture2_);

	delete vao_;
	delete vbo_;
	delete ebo_;
}

void DrawRectangle::Init(const void* context)
{

	vao_->Bind();
	vbo_->Bind();

	float rectangleVertices[] = {

		////����λ��				//��ɫ				// ��������
		//0.5f, 0.5f, 0.0f,		1.0f,0.0f,0.0f,		2.0f, 2.0f,		0.3f, 0.3f,// ���Ͻ�
		//0.5f, -0.5f, 0.0f,		0.0f,1.0f,0.0f,		2.0f, 0.0f,		0.3f, 0.0f,// ���½�
		//-0.5f, -0.5f, 0.0f,		0.0f,0.0f,1.0f,		0.0f, 0.0f,		0.0f, 0.0f,// ���½�
		//-0.5f, 0.5f, 0.0f,		1.0f,1.0f,0.0f,		0.0f, 2.0f,		0.0f, 0.1f,// ���Ͻ�

		//����λ��				//��ɫ				// ��������
		0.5f, 0.5f, 0.0f,		1.0f,0.0f,0.0f,		2.0f, 2.0f,		1.0f, 1.0f,// ���Ͻ�
		0.5f, -0.5f, 0.0f,		0.0f,1.0f,0.0f,		2.0f, 0.0f,		1.0f, 0.0f,// ���½�
		-0.5f, -0.5f, 0.0f,		0.0f,0.0f,1.0f,		0.0f, 0.0f,		0.0f, 0.0f,// ���½�
		-0.5f, 0.5f, 0.0f,		1.0f,1.0f,0.0f,		0.0f, 2.0f,		0.0f, 1.0f,// ���Ͻ�
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
	//vbo_->SetBufferData(size, vertices, GL_STATIC_DRAW);
	ebo_->Bind();

	//���������εĶ���λ��
	uint indices[] = {
		0,1,3, //��һ��������
		1,2,3, //�ڶ���������
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));	
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1_);

	//����������openGL,���������һ��2d������PS:2d���������Ϊs t
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//��������ͼƬ
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	uchar* image = stbi_load("./assets/textures/container.jpg", &width, &height, &nrChannels, 0);

	if (image) 
	{
		//�ڵ�ǰ�İ�����������ڻ����Ԫ��������
		//glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
		//target: ָ�������Ŀ�����ͣ�GL_TEXTURE_1D,GL_TEDTURE_2D,GL_TEXTURE_3D
		//level:  ָ���༶��Զ����ļ���0Ϊ����ͼ�񼶱�n��ʾ��n��mipmap����СͼƬ
		//internalFormat: ָ��������ĸ�ʽ������ͻ��ֻ��RGB�ʹ洢ΪRGBֵ��
		//width:  ָ������Ŀ��
		//height: ָ������ͼƬ�ĸ߶ȣ��������������еĲ���
		//border: ����Ϊ0��api����ʷ��������
		//format: ָ��ԭͼ�����ݸ�ʽ������ԭͼֻ��RGB,���������RGB,���������alphaͨ������Ҫ��pngͼƬ�������GL_RGBA	
		//type:   ָ��ԭͼ����������
		//data:	  ͼƬ����ָ��
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		//��������
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	//�������������úú󣬼ǵ�Ҫ�ͷ�
	stbi_image_free(image);

	//glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	image = stbi_load("./assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	//�������������úú󣬼ǵ�Ҫ�ͷ�
	stbi_image_free(image);

	shader_->Use();
	shader_->SetInt("ourTexture1", 0);
	shader_->SetInt("ourTexture2", 1);
}

void DrawRectangle::Update(const void* context)
{

	//����ж��������Ҫ��openGL���ж��壬�������ĸ�������Ԫ�������һ����������Ҫ��openGL��������16��,0-15
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2_);

	shader_->Use();
	//shader_->SetInt("ourTexture1", 0);
	//shader_->SetInt("ourTexture2", 1);

	shader_->Set2f("towards", -1.0f, -1.0f);
	DrawContext* c= (DrawContext*)context;
	
	shader_->SetFloat("visibility", Clamp01(visibilityValue_));
	mat4 trans = mat4(1.0f);

	trans = glm::translate(trans, vec3(0.5f, -0.5f, 0.0f));

	trans = glm::rotate(trans, c->time_, vec3(0, 0, 1.0f));
	shader_->SetMatrix4fv("transform", 1, GL_FALSE, glm::value_ptr(trans));

	vao_->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	trans = glm::translate(trans, vec3(-0.5f, 0.5f, 0.0f));
	float value = sin(c->time_);
	value = ABS(value);
	trans = glm::scale(trans, vec3(1.0f,1.0f, 1.0f) * value);
	trans = glm::rotate(trans, c->time_, vec3(0.0f, 1.0f, 0.0f));
	shader_->SetMatrix4fv("transform", 1, GL_FALSE, glm::value_ptr(trans));

	vao_->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void DrawRectangle::InputDrive(GLFWwindow* window, float time, float deltaTime) 
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		visibilityValue_ += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		visibilityValue_ -= 0.1f;
	}
}

DrawTwoConnectTriangle::DrawTwoConnectTriangle(Shader* shader):DrawBase(shader)
{
	vao_ = new VAOContext();
	vbo_ = new VBOContext();
}

DrawTwoConnectTriangle::~DrawTwoConnectTriangle() 
{
	//ģʽѡ�����(Ĭ�ϣ������������)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	delete vao_;
	delete vbo_;
}

void DrawTwoConnectTriangle::Init(const void* context)
{
	vao_->Bind();
	vbo_->Bind();

	float vertices[] =
	{
		-1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,
		-0.5f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,1.0f, 0.0f, 1.0f,
		0.5f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,0.0f, 1.0f, 0.0f,
		0.5f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,
		-0.5f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f,
	};

	vbo_->SetBufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//ģʽѡ���߿�
	//�ú�����������OpenGL��λ���ͼԪ
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void DrawTwoConnectTriangle::Update(const void* context)
{
	shader_->Use();
	vao_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 9);
}

DrawTwoTriangleUseDiffVAOandVBO::DrawTwoTriangleUseDiffVAOandVBO(Shader* shader):DrawBase(shader)
{
	glGenBuffers(2, vbos_);
	glGenVertexArrays(2, vaos_);
}

DrawTwoTriangleUseDiffVAOandVBO::~DrawTwoTriangleUseDiffVAOandVBO() 
{
	glDeleteBuffers(2, vbos_);
	glDeleteVertexArrays(2, vaos_);
}

void DrawTwoTriangleUseDiffVAOandVBO::Init(const void* context)
{
	float vertices[] = {
		//λ��				//��ɫ
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// left  
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right 
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,// top   
	};

	float secondVertices[] = {
		0.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f,// left
		0.5f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f, // right 
		1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,// top 
	};
	glBindBuffer(GL_ARRAY_BUFFER, vbos_[0]);
	glBindVertexArray(vaos_[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbos_[1]);
	glBindVertexArray(vaos_[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(secondVertices), secondVertices, GL_DYNAMIC_DRAW);
	//because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void DrawTwoTriangleUseDiffVAOandVBO::Update(const void* context)
{
	shader_->Use();
	glBindVertexArray(vaos_[0]);

	DrawContext* c = (DrawContext*)context;

	shader_->SetBoolean("useOffset", true);
	float valueX = sin(c->time_);
	float valueY = cos(c->time_);
	shader_->Set3f("aPosXYZ", valueX, valueY, 0.0f);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vaos_[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

DrawDynamicTriangle::DrawDynamicTriangle(Shader* shader):DrawBase(shader)
{
	vao_ = new VAOContext();
	vbo_ = new VBOContext();
}

DrawDynamicTriangle::~DrawDynamicTriangle()
{
	delete vao_;
	delete vbo_;
}

void DrawDynamicTriangle::Init(const void* context)
{

}

void DrawDynamicTriangle::Update(const void* context)
{
	vao_->Bind();
	vbo_->Bind();

	float vertices[18] = {
		//λ��				//��ɫ
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// left  
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right 
			0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,// top  
	};
	
	DrawContext* c = (DrawContext*)context;

	float cosValue = cos(c->time_);
	float sinValue = sin(c->time_);

	int index = 0;
	vertices[index] = sinValue;
	vertices[index + 1] = cosValue;

	index += 6;
	vertices[index] = sinValue * 0.5f;
	vertices[index + 1] = cosValue * 0.5f;

	index += 6;

	vertices[index] = 0 - cosValue;
	vertices[index + 1] = 0 - cosValue ;


	index = 3;

	vertices[index] = sinValue;
	vertices[index + 1] = cosValue;
	vertices[index + 2] = cosValue;

	index += 6;
	vertices[index] = sinValue * 0.5f;
	vertices[index + 1] = cosValue * 0.5f;
	vertices[index + 2] = cosValue * 0.5f;

	index += 6;

	vertices[index] = 0 - sinValue;
	vertices[index + 1] = 0 - cosValue;
	vertices[index + 2] = 0 - cosValue;

	vbo_->SetBufferData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	vbo_->Bind();

	shader_->Use();

	float valueX = sin(c->time_);
	float valueY = cos(c->time_);

	shader_->SetBoolean("useOffset", true);
	shader_->Set3f("aPosXYZ", valueX, valueY, 0.0f);

	vao_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}


Draw3D::Draw3D(Shader* shader) :DrawBase(shader)
{
	vao_ = new VAOContext();
	ebo_ = new EBOContext();
	vbo_ = new VBOContext();

	glGenTextures(1, &texture1_);
	glGenTextures(1, &texture2_);
}

Draw3D::~Draw3D()
{
	glDeleteTextures(1, &texture1_);
	glDeleteTextures(1, &texture2_);

	delete vao_;
	delete ebo_;
	delete vbo_;
}

void Draw3D::Init(const void* context)
{


	//OpenGL ͨ��glEnable �� glDisable �����ƿ�����ر�һЩ���ܣ�ֱ�����ö�Ӧ�Ĺ������ر�Ϊֹ
	//����ѡ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	vao_->Bind();
	vbo_->Bind();

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
	};


	//float vertices[] = {
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	//};


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	ebo_->Bind();

	uint indices[] =
	{
		0,1,3,
		1,2,3,
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//������
	glBindTexture(GL_TEXTURE_2D, texture1_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	uchar* image = stbi_load("./assets/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, texture2_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	image = stbi_load("./assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);


	shader_->Use();
	shader_->SetInt("ourTexture1", 0);
	shader_->SetInt("ourTexture2", 1);
}

void Draw3D::Update(const void* context)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2_);

	shader_->Use();
	
	DrawContext* c = (DrawContext*)context;

	// document: < https://learnopengl-cn.github.io/01%20Getting%20started/08%20Coordinate%20Systems/#_2 >
	////����һ��ģ�;���
	//mat4 model = mat4(1.0f);
	//model = glm::rotate(model, glm::radians(50.0f)*time, vec3(0.5f, 1.0f, 0.0f));

	////����һ���۲����
	mat4 view = mat4(1.0f);
	//����Ҫ��ģ�;���λ�ƣ�������Ϊ��һ�����
	view = glm::translate(view, vec3(0.0f, 0.0f, -3.0f));

	//����һ��͸�Ӿ�֤��ʹ��͸��ͶӰ
	mat4 projection;
	projection = glm::perspective(glm::radians(45.0f),((float)c->windowWidth_ / (float)c->windowHeight_), 0.01f, 100.0f);

	//shader_->SetMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
	shader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));

	//shader_->Set2f("towards", -1.0f, -1.0f);


	shader_->SetFloat("visibility", Clamp01(visibilityValue_));

	vao_->Bind();

	for (uint i = 0; i < 10; i++) 
	{
		mat4 model;
		model = glm::translate(model, cubePositions_[i]);
		float angle = 0.0f;
		angle = 20.0f * (i + 1);
		model = glm::rotate(model, glm::radians(angle)*c->time_, vec3(1.0f, 0.3f, 0.5f));
		shader_->SetMatrix4fv("model",1,GL_FALSE,&model[0][0]);


		//��OpenGL�У���Ⱦһ�����������еĶ��㶼Ҫ����һ�µĿռ�����ת��:�ֲ�����->��������->�۲�����->�ü�����->��Ļ����Ĺ��̣���Щ�Ĺ��̶�����ͨ������ı仯��ʵ��ת��
		//��OpenGL�У������ṩ���������ɵ��ü����꼴�ɣ�OpenGL���������ͨ��ʹ��glViewPort����׼���豸���굽��Ļ����
		//�ü����꼴��͸�Ӿ���(�ü��ռ�) * �۲����(�۲�ռ�) * �������(����ռ�) * �ֲ�����(�ֲ��ռ�)  V clip = M projection * M view * M model * M local (һ��Ҫ��������) ���󲻷��Ͻ�����
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void Draw3D::InputDrive(GLFWwindow* window, float time, float deltaTime)
{

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		visibilityValue_ += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		visibilityValue_ -= 0.1f;
	}
}

DrawCamera::DrawCamera(Shader* shader, FPS_Camera* camera) :DrawBase(shader, camera)
{
	vao_ = new VAOContext();
	ebo_ = new EBOContext();
	vbo_ = new VBOContext();

	glGenTextures(1, &texture1_);
	glGenTextures(1, &texture2_);
}

DrawCamera::~DrawCamera()
{
	glDeleteTextures(1, &texture1_);
	glDeleteTextures(1, &texture2_);

	delete vao_;
	delete ebo_;
	delete vbo_;
}

void DrawCamera::Init(const void* context)
{
	
	//OpenGL ͨ��glEnable �� glDisable �����ƿ�����ر�һЩ���ܣ�ֱ�����ö�Ӧ�Ĺ������ر�Ϊֹ
	//����ѡ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	vao_->Bind();
	vbo_->Bind();

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
	};

	//float vertices[] = {
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	//};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	ebo_->Bind();

	uint indices[] =
	{
		0,1,3,
		1,2,3,
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//������
	glBindTexture(GL_TEXTURE_2D, texture1_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	uchar* image = stbi_load("./assets/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, texture2_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	image = stbi_load("./assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);


	shader_->Use();
	shader_->SetInt("ourTexture1", 0);
	shader_->SetInt("ourTexture2", 1);
}

void DrawCamera::Update(const void* context)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2_);

	shader_->Use();

	DrawContext* c = (DrawContext*)context;

	// document: < https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/#_1 >
	////����һ��ģ�;���
	//mat4 model = mat4(1.0f);
	//model = glm::rotate(model, glm::radians(50.0f)*time, vec3(0.5f, 1.0f, 0.0f));
	
	float speed = cameraSpeed_  * c->delaTime_;

	mat4 view = mat4(1.0f);
	//lookAt(eye,center,up)
	//eye: ���λ��
	//center:���ָ���Ŀ��λ��
	//up:���������е���������
	//��������ľ���Ϳ���������Ϊ�۲����
	view = glm::lookAt(cameraPos_, cameraPos_ + cameraFront_, cameraUp_);
	mat4 v = camera_->GetViewMatrix();
	//std::cout << "�۲��֤ view: \n{\n " << view[0][0] << "," << view[0][1] << "," << view[0][2] << "," << view[0][3] << ",\n"
	//	<< view[1][0] << "," << view[1][1] << "," << view[1][2] << "," << view[1][3] << ",\n" << view[2][0] << "," << view[2][1] << "," << view[2][2] << "," << view[2][3]
	//	<< "\n" << view[3][0] << "," << view[3][1] << "," << view[3][2] << "," << view[3][3] << "\n}";

	mat4 projection = mat4(1.0f);
	projection = camera_->GetProjectionMatrix(c->aspectRatio_);

	shader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(v));
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));

	//shader_->Set2f("towards", -1.0f, -1.0f);

	shader_->SetFloat("visibility", Clamp01(visibilityValue_));

	vao_->Bind();

	for (uint i = 0; i < 10; i++)
	{
		mat4 model = mat4(1.0f);
		model = glm::translate(model, cubePositions_[i]);
		float angle = 0.0f;
		angle = 20.0f * (i + 1);
		model = glm::rotate(model, glm::radians(angle) * c->time_, vec3(1.0f, 0.3f, 0.5f));
		shader_->SetMatrix4fv("model", 1, GL_FALSE, &model[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void DrawCamera::InputDrive(GLFWwindow* window, float time, float deltaTime) 
{

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		visibilityValue_ += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		visibilityValue_ -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::LEFT, deltaTime);
	}
}

void DrawCamera::MoveCamera(float xpos, float ypos, bool constrainPitch) 
{
	if (firstMouse_) 
	{
		lastX_ = xpos;
		lastY_ = ypos;
		firstMouse_ = false;
	}

	float xoffset = xpos - lastX_;
	float yoffset = lastY_ - ypos;

	lastX_ = xpos;
	lastY_ = ypos;

	camera_->ProcessMouseMovement(xoffset, yoffset, constrainPitch);

}

DrawSphere::DrawSphere(Shader* shader, FPS_Camera* camera) :DrawBase(shader, camera)
{
	vao_ = new VAOContext();
	ebo_ = new EBOContext();
	vbo_ = new VBOContext();
}

DrawSphere::~DrawSphere()
{
	delete vao_;
	delete ebo_;
	delete vbo_;
}

void DrawSphere::Init(const void* context)
{
	 
	//w->RegisterMousePosition(([](GLFWwindow* window, double xpos, double ypos) {Mouse_Callback(window, xpos, ypos);}));

	//OpenGL ͨ��glEnable �� glDisable �����ƿ�����ر�һЩ���ܣ�ֱ�����ö�Ӧ�Ĺ������ر�Ϊֹ
	//����ѡ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	vao_->Bind();
	vbo_->Bind();

	std::vector<float> vertices;

	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*count, vertices, GL_DYNAMIC_DRAW);

	ebo_->Bind();

	uint indices[] =
	{
		0,1,3,
		1,2,3,
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void DrawSphere::Update(const void* context)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_->Use();

	DrawContext* c = (DrawContext*)context;

	// document: < https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/#_1 >
	////����һ��ģ�;���
	//mat4 model = mat4(1.0f);
	//model = glm::rotate(model, glm::radians(50.0f)*time, vec3(0.5f, 1.0f, 0.0f));

	mat4 view = mat4(1.0f);
	//lookAt(eye,center,up)
	//eye: ���λ��
	//center:���ָ���Ŀ��λ��
	//up:���������е���������
	//��������ľ���Ϳ���������Ϊ�۲����
	view = glm::lookAt(cameraPos_, cameraPos_ + cameraFront_, cameraUp_);
	mat4 v = camera_->GetViewMatrix();
	//std::cout << "�۲��֤ view: \n{\n " << view[0][0] << "," << view[0][1] << "," << view[0][2] << "," << view[0][3] << ",\n"
	//	<< view[1][0] << "," << view[1][1] << "," << view[1][2] << "," << view[1][3] << ",\n" << view[2][0] << "," << view[2][1] << "," << view[2][2] << "," << view[2][3]
	//	<< "\n" << view[3][0] << "," << view[3][1] << "," << view[3][2] << "," << view[3][3] << "\n}";

	mat4 projection = mat4(1.0f);
	projection = camera_->GetProjectionMatrix(c->aspectRatio_);

	shader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(v));
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));

	//shader_->Set2f("towards", -1.0f, -1.0f);

	shader_->SetFloat("visibility", Clamp01(visibilityValue_));

	vao_->Bind();

}

void DrawSphere::InputDrive(GLFWwindow* window, float time, float deltaTime) 
{
	float speed = cameraSpeed_ * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		visibilityValue_ += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		visibilityValue_ -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::LEFT, deltaTime);
	}
}

void DrawSphere::MoveCamera(float xpos, float ypos, bool constrainPitch)
{
	if (firstMouse_)
	{
		lastX_ = xpos;
		lastY_ = ypos;
		firstMouse_ = false;
	}

	float xoffset = xpos - lastX_;
	float yoffset = lastY_ - ypos;

	lastX_ = xpos;
	lastY_ = ypos;

	camera_->ProcessMouseMovement(xoffset, yoffset, constrainPitch);

}

LightDrawBase::LightDrawBase(Shader* cubeShader, Shader* lightShader, FPS_Camera* camera) :DrawBase(cubeShader, camera),lightShader_(lightShader)
{
	vao_ = new VAOContext();
	lightVAO_ = new VAOContext();
	vbo_ = new VBOContext();
}

LightDrawBase::~LightDrawBase()
{
	delete vao_;
	delete lightVAO_;
	delete vbo_;
}

void LightDrawBase::MoveCamera(float xpos, float ypos, bool constrainPitch) 
{
	if (firstMouse_)
	{
		lastX_ = xpos;
		lastY_ = ypos;
		firstMouse_ = false;
	}

	float xoffset = xpos - lastX_;
	float yoffset = lastY_ - ypos;

	lastX_ = xpos;
	lastY_ = ypos;

	camera_->ProcessMouseMovement(xoffset, yoffset, constrainPitch);
}

void LightDrawBase::InputDrive(GLFWwindow* window,float time,float delaTime)
{
	float speed = cameraSpeed_ * delaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::FORWARD, delaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::BACKWARD, delaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::RIGHT, delaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::LEFT, delaTime);
	}
}

LightDraw::LightDraw(Shader* shader, Shader* lightShader, FPS_Camera* camera) :LightDrawBase(shader, lightShader, camera) {}

LightDraw::~LightDraw() {}

void LightDraw::Init(const void* context) 
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 1.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f, 1.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 0.0f, 0.0f,
			
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 0.0f, 0.0f,
	};

	vao_->Bind();
	vbo_->Bind();

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	//���İ󶨣��󶨹��յ�VAO
	lightVAO_->Bind();
	vbo_->Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
}

void LightDraw::Update(const void* context) 
{
	DrawContext* c = (DrawContext*)context;
	
	vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
	vec3 lightColor = vec3(1.0f,1.0f,1.0f);
	//vec3 lightColor = vec3(0.341f, 0.945f, 0.976f);
	lightColor.x = Abs(sin(c->time_) * 2.0f);
	lightColor.y = Abs(sin(c->time_) * 0.7f);
	lightColor.z = Abs(sin(c->time_ * 1.3f));
	vec3 cubeColor = vec3(1.0f, 0.5f, 0.31f);
	//vec3 cubeColor = vec3(1.0f, 1.0f, 1.0f);

	//lightPos.x = 1.0f + sin(c->time_) * 2.0f;
	//lightPos.y = sin(c->time_/2) * 1.0f;

	glClear(GL_DEPTH_BUFFER_BIT);

	//����3D������Ĺ�����ͬ�Ĺ۲졢͸�Ӿ���model�Լ���������
	mat4 model = mat4(1.0f);
	model = glm::rotate(model, glm::radians(c->time_ * 20.0f), vec3(0.0f, 1.0f, 0.0f));
	mat3 normalMatrix = mat3(1.0);
	//���߾�����CPU������м��� G = (M^-1)^T
	normalMatrix = glm::transpose(glm::inverse(model));
	model = glm::scale(model, vec3(5.0f, 0.2f, 5.0f));
	mat4 projection = camera_->GetProjectionMatrix(c->aspectRatio_);
	mat4 view = camera_->GetViewMatrix();

	vec3 diffuseColor = lightColor * vec3(0.5f);
	vec3 ambientColor = diffuseColor * vec3(0.2f);

	//��һ�����Ƶ�һ��vao
	shader_->Use();

	shader_->Set3fv("light.position", &lightPos[0]);
	shader_->Set3f("light.ambient", 0.2f, 0.2f, 1.0f);
	shader_->Set3fv("light.diffuse", VALUE_PTR(diffuseColor));
	shader_->Set3fv("light.specular", VALUE_PTR(ambientColor));

	shader_->Set3fv("objectColor", VALUE_PTR(cubeColor));

	shader_->SetMatrix4fv("model", 1, GL_FALSE, &model[0][0]);
	shader_->SetMatrix3fv("normalMatrix", 1, GL_FALSE, &normalMatrix[0][0]);
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));
	shader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
	shader_->Set3fv("viewPos", &camera_->position[0]);

	shader_->Set3f("material.specular", 0.5f,0.5f,0.5f);//���þ���ǿ��
	shader_->Set3f("material.diffuse", 1.0f, 0.5f, 0.31f);
	shader_->Set3f("material.ambient", 1.0f, 0.5f, 0.31f);
	shader_->SetFloat("material.shininess", 32.0f);//���÷����

	vao_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//���ƹ�Դ���õڶ���vao
	lightShader_->Use();

	model = mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, vec3(0.2f));

	lightShader_->Set3fv("lightColor", VALUE_PTR(lightColor));
	lightShader_->SetMatrix4fv("model", 1, GL_FALSE, VALUE_PTR(model));
	lightShader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));
	lightShader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
	lightVAO_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

LightingMapsDraw::LightingMapsDraw(Shader* shader, Shader* lightShader, FPS_Camera* camera) :LightDrawBase(shader, lightShader, camera)
{
	glGenTextures(1, &diffuseTex_);
	glGenTextures(1, &specularTex_);
	glGenTextures(1, &emissionTex_);
}

LightingMapsDraw::~LightingMapsDraw()
{
	glDeleteTextures(1, &diffuseTex_);
	glDeleteTextures(1, &specularTex_);
	glDeleteBuffers(1, &emissionTex_);
}

void LightingMapsDraw::Init(const void* context)
{
	
	vao_->Bind();
	vbo_->Bind();

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	bool success = LoadImage2D(&diffuseTex_, "./assets/textures/container2.png", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	success = LoadImage2D(&specularTex_, "./assets/textures/container2_specular.png", GL_REPEAT, GL_NEAREST);
	//��ϰ3
	//success = LoadImage2D(&specularTex_, "./assets/textures/lighting_maps_specular_color.png", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	success = LoadImage2D(&emissionTex_, "./assets/textures/matrix.jpg", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	lightVAO_->Bind();
	vbo_->Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	shader_->Use();
	shader_->SetInt("material.diffuse", 0);
	shader_->SetInt("material.specular", 1);
	shader_->SetInt("material.emission", 2);
}

void LightingMapsDraw::Update(const void* context)
{

	DrawContext* c = (DrawContext*)context;

	vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	//lightPos.x = 1.0f + sin(c->time_) * 2.0f;
	//lightPos.y = sin(c->time_/2) * 1.0f;

	glClear(GL_DEPTH_BUFFER_BIT);

	mat4 model = mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f) * sin(c->time_ / 2) * 1.0f);
	model = glm::rotate(model, glm::radians(c->time_ * 10.0f), vec3(0.0f, 1.0f, 0.0f));

	mat3 normalMatrix = mat3(1.0);
	normalMatrix = glm::transpose(glm::inverse(model));
	//model = glm::scale(model, vec3(5.0f, 0.2f, 5.0f));
	mat4 projection = camera_->GetProjectionMatrix(c->aspectRatio_);
	mat4 view = camera_->GetViewMatrix();

	vec3 diffuseColor = lightColor * vec3(0.5f);
	vec3 ambientColor = lightColor * vec3(2.0f);
	vec3 ambient = lightColor * vec3(0.5f);

	//��һ�����Ƶ�һ��vao
	shader_->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularTex_);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionTex_);

	shader_->Set3fv("light.position", &lightPos[0]);
	shader_->Set3fv("light.ambient", &ambient[0]);
	shader_->Set3fv("light.diffuse", VALUE_PTR(diffuseColor));
	shader_->Set3fv("light.specular", VALUE_PTR(ambientColor));

	shader_->SetMatrix4fv("model", 1, GL_FALSE, &model[0][0]);
	shader_->SetMatrix3fv("normalMatrix", 1, GL_FALSE, &normalMatrix[0][0]);
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));
	shader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
	shader_->Set3fv("viewPos", &camera_->position[0]);

	shader_->SetFloat("material.shininess", 64.0f);
	shader_->Set2f("emissionOffset", 0.0f, c->time_ *0.5f);

	vao_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//���ƹ�Դ���õڶ���vao
	lightShader_->Use();

	model = mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, vec3(0.2f));

	lightShader_->SetMatrix4fv("model", 1, GL_FALSE, VALUE_PTR(model));
	lightShader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));
	lightShader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
	lightShader_->Set3fv("lightColor", &lightColor[0]);

	lightVAO_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

Lightcasters_DirectionalLight_Draw::Lightcasters_DirectionalLight_Draw(Shader* cubeShader, Shader* lightShader, FPS_Camera* camera) :LightingMapsDraw(cubeShader, lightShader, camera) {}
Lightcasters_DirectionalLight_Draw::~Lightcasters_DirectionalLight_Draw() {}

void Lightcasters_DirectionalLight_Draw::Init(const void* context) 
{
	vao_->Bind();
	vbo_->Bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	bool success = LoadImage2D(&diffuseTex_, "./assets/textures/container2.png", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	success = LoadImage2D(&specularTex_, "./assets/textures/container2_specular.png", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	success = LoadImage2D(&emissionTex_, "./assets/textures/matrix.jpg", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	lightVAO_->Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	shader_->Use();
	shader_->SetInt("material.diffuse", 0);
	shader_->SetInt("material.specular", 1);
	shader_->SetInt("material.emission", 2);
}

void Lightcasters_DirectionalLight_Draw::Update(const void* context)
{
	DrawContext* c = (DrawContext*)context;
	glClear(GL_DEPTH_BUFFER_BIT);

	Light light;
	light.color = vec3(1.0f, 1.0f, 1.0f);
	vec3 normalVec3 = vec3(1.0f, 1.0f, 1.0f);
	light.direction = vec3(-0.2f, -1.0f, -0.3f);
	light.ambient = normalVec3 * vec3(0.2f);
	light.diffuse = normalVec3 * vec3(0.5f);
	light.specular = normalVec3 * vec3(2.0f);
	
	shader_->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularTex_);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionTex_);


	shader_->Set3fv("light.direction", VALUE_PTR(light.direction));
	shader_->Set3fv("light.ambient", &light.ambient[0]);
	shader_->Set3fv("light.diffuse", &light.diffuse[0]);
	shader_->Set3fv("light.specular", &light.specular[0]);

	mat4 projection = camera_->GetProjectionMatrix(c->aspectRatio_);
	mat4 view = camera_->GetViewMatrix();

	shader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));
	shader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));

	shader_->Set3fv("viewPos", &camera_->position[0]);

	shader_->SetFloat("material.shininess", 32.0f);
	shader_->Set2f("emissionOffset", 0.0f, sin(c->time_ / 2));

	vao_->Bind();

	for (int i = 0;i < 10;i++) 
	{
		mat4 model = mat4(1.0f);
		model = glm::translate(model, cubePositions_[i]);
		float angle = 0.0f;
		angle = 20.0f * (i + 1);
		model = glm::rotate(model, glm::radians(angle) * c->time_, vec3(1.0f, 0.3f, 0.5f));

		mat3 normalMat = mat3(1.0f);
		normalMat = glm::transpose(glm::inverse(model));

		shader_->SetMatrix4fv("model", 1, GL_FALSE, VALUE_PTR(model));
		shader_->SetMatrix3fv("normalMatrix", 1, GL_FALSE, VALUE_PTR(normalMat));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	lightVAO_->Bind();
	mat4 model = mat4(1.0f);
	model = glm::translate(model, vec3(1.2f, 1.0f, 2.0f));
	model = glm::scale(model, vec3(0.2f));

	vec3 lightDir = vec3(0.0f) - light.direction;
	mat4 lightPos = glm::translate(model, lightDir);

	lightShader_->Use();
	lightShader_->SetMatrix4fv("model", 1, GL_FALSE, VALUE_PTR(model));
	lightShader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));
	lightShader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
	lightShader_->Set3fv("lightColor", &light.color[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

Lightcasters_PointLight_Draw::Lightcasters_PointLight_Draw(Shader* cubeShader, Shader* lightShader, FPS_Camera* camera) :Lightcasters_DirectionalLight_Draw(cubeShader, lightShader, camera) {}
Lightcasters_PointLight_Draw::~Lightcasters_PointLight_Draw() {}

void Lightcasters_PointLight_Draw::SetLightShaderValue(Light light)
{
	shader_->Set3fv("light.position", VALUE_PTR(light.position));
	shader_->Set3fv("light.ambient", VALUE_PTR(light.ambient));
	shader_->Set3fv("light.diffuse", VALUE_PTR(light.diffuse));
	shader_->Set3fv("light.specular", VALUE_PTR(light.specular));

	shader_->SetFloat("light.k_constant", light.k_constant);
	shader_->SetFloat("light.k_linear", light.k_linear);
	shader_->SetFloat("light.k_quadratic", light.k_quadratic);
}

void Lightcasters_PointLight_Draw::Update(const void* context) 
{
	DrawContext* c = (DrawContext*)context;
	glClear(GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularTex_);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionTex_);

	Light light;
	light.position = vec3(1.2f, 1.0f, 2.0f);
	light.ambient = normal * vec3(0.2f);
	light.color = normal;
	light.diffuse = normal * vec3(0.5f);
	light.specular = normal * vec3(1.0f);
	light.k_constant = 1.0f;
	light.k_linear = 0.09f;
	light.k_quadratic = 0.032f;

	shader_->Use();
	SetLightShaderValue(light);
	shader_->SetFloat("material.shininess", 32.0f);


	mat4 view = camera_->GetViewMatrix();
	mat4 projection = camera_->GetProjectionMatrix(c->aspectRatio_);

	shader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
	shader_->Set3fv("viewPos", &camera_->position[0]);
	shader_->Set2f("emissionOffset", 0.0f, (c->time_ / 2));
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));

	vao_->Bind();
	for (int i = 0;i < 10;i++) 
	{
		mat4 model = mat4(1.0f);
		mat3 normalMat = mat3(1.0f);

		float angle = 20.0f * i;
		model = glm::translate(model, cubePositions_[i]);
		model = glm::rotate(model, glm::radians(angle) * c->time_ / (i + 1), vec3(1.0f, 0.3f, 0.5f));

		normalMat = glm::transpose(glm::inverse(model));
		shader_->SetMatrix4fv("model", 1, GL_FALSE, VALUE_PTR(model));
		shader_->SetMatrix3fv("normalMatrix", 1, GL_FALSE, VALUE_PTR(normalMat));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	lightVAO_->Bind();
	mat4 model = mat4(1.0f);
	model = glm::translate(model, light.position);
	model = glm::scale(model, vec3(0.2f));

	lightShader_->Use();
	lightShader_->SetMatrix4fv("model", 1, GL_FALSE, VALUE_PTR(model));
	lightShader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));
	lightShader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
	lightShader_->Set3fv("lightColor", &light.color[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

Lightcasters_Spotlight_Draw::Lightcasters_Spotlight_Draw(Shader* cubeShader, Shader* lightShader, FPS_Camera* camera) :Lightcasters_DirectionalLight_Draw(cubeShader, lightShader, camera) 
{
	glGenTextures(1, &lightTexture_);
}
Lightcasters_Spotlight_Draw::~Lightcasters_Spotlight_Draw() 
{
	glDeleteTextures(1, &lightTexture_);
}

void Lightcasters_Spotlight_Draw::Init(const void* context)
{
	vao_->Bind();
	vbo_->Bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	bool success = LoadImage2D(&diffuseTex_, "./assets/textures/container2.png", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	success = LoadImage2D(&specularTex_, "./assets/textures/container2_specular.png", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	success = LoadImage2D(&emissionTex_, "./assets/textures/matrix.jpg", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	success = LoadImage2D(&lightTexture_, "./assets/textures/girl.jpg", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	lightVAO_->Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	shader_->Use();
	shader_->SetInt("material.diffuse", 0);
	shader_->SetInt("material.specular", 1);
	shader_->SetInt("material.emission", 2);
	shader_->SetInt("light.texture", 3);
}

void Lightcasters_Spotlight_Draw::Update(const void* context) 
{
	DrawContext* c = (DrawContext*)context;
	glClear(GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularTex_);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionTex_);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, lightTexture_);

	Light light;
	light.position = vec3(1.2f, sin(c->time_), 2.0f);

	vec3 lightRight = glm::cross(light.position, vec3(0.0f, 1.0f, 0.0f));
	//vec3 lightDir = cubePositions_[0] - light.position;
	vec3 lightDir = glm::cross(lightRight, vec3(0.0f, 1.0f, 0.0f));
	light.color = vec3(1.0f,0.0f,0.0f);
	light.direction = glm::normalize(lightDir);
	light.ambient = normal * vec3(0.2f);
	light.diffuse = light.color * vec3(0.5f);
	light.specular = light.color * vec3(3.0f);
	light.k_constant = 1.0f;
	light.k_linear = 0.09f;
	light.k_quadratic = 0.032f;
	//�������ǱȽϽǶȵĴ�С��ͨ������lightDir��SpotDir�������ڻ������ص���һ��cosֵ����gpu�м��㷴�����Ǻܲ����ǵģ���ˣ���CPU������м�������ֵȻ��Ƚ�����ֵ���ɡ�
	light.innterCutOff = glm::cos(glm::radians(12.5f));
	light.outerCutOff = glm::cos(glm::radians(17.5f));

	shader_->Use();
	shader_->SetFloat("material.shininess", 256.0f);

	shader_->Set3fv("light.position", VALUE_PTR(light.position));
	shader_->Set3fv("light.direction", VALUE_PTR(light.direction));
	shader_->Set3fv("light.ambient", VALUE_PTR(light.ambient));
	shader_->Set3fv("light.diffuse", VALUE_PTR(light.diffuse));
	shader_->Set3fv("light.specular", VALUE_PTR(light.specular));

	shader_->SetFloat("light.k_constant", light.k_constant);
	shader_->SetFloat("light.k_linear", light.k_linear);
	shader_->SetFloat("light.k_quadratic", light.k_quadratic);

	shader_->SetFloat("light.innterCutOff", light.innterCutOff);
	shader_->SetFloat("light.outerCutOff", light.outerCutOff);

	mat4 view = camera_->GetViewMatrix();
	mat4 projection = camera_->GetProjectionMatrix(c->aspectRatio_);

	shader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
	shader_->Set3fv("viewPos", VALUE_PTR(camera_->position));
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));
	shader_->Set2f("emissionOffset", 0.0f, sin(c->time_ / 2));

	vao_->Bind();
	for (int i = 0;i < 10;i++)
	{
		mat4 model = mat4(1.0f);
		mat3 normalMat = mat3(1.0f);

		float angle = 20.0f * i;
		model = glm::translate(model, cubePositions_[i]);
		model = glm::rotate(model, glm::radians(angle) * c->time_, vec3(1.0f, 0.3f, 0.5f));

		normalMat = glm::transpose(glm::inverse(model));
		shader_->SetMatrix4fv("model", 1, GL_FALSE, VALUE_PTR(model));
		shader_->SetMatrix3fv("normalMatrix", 1, GL_FALSE, VALUE_PTR(normalMat));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	lightVAO_->Bind();
	lightShader_->Use();
	mat4 lightMat = mat4(1.0f);
	lightMat = glm::translate(lightMat, light.position);
	lightMat = glm::scale(lightMat, vec3(0.2f));

	lightShader_->Use();
	lightShader_->SetMatrix4fv("model", 1, GL_FALSE, VALUE_PTR(lightMat));
	lightShader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));
	lightShader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
	lightShader_->Set3fv("lightColor", &light.color[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

}


MultiLight::MultiLight(Shader* cubeShader, Shader* lightShader, FPS_Camera* camera) :Lightcasters_DirectionalLight_Draw(cubeShader, lightShader, camera) ,texturesCount_(4)
{
	textures_ = new uint[texturesCount_];
	glGenTextures(texturesCount_, textures_);
}
MultiLight::~MultiLight() 
{
	glDeleteTextures(texturesCount_, textures_);
	delete[] textures_;
}

void MultiLight::Init(const void* context)
{
	vao_->Bind();
	vbo_->Bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	bool success = LoadImage2D(textures_, "./assets/textures/container2.png", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	success = LoadImage2D(textures_+1, "./assets/textures/container2_specular.png", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	success = LoadImage2D(&textures_[2], "./assets/textures/matrix.jpg", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	success = LoadImage2D(&textures_[3], "./assets/textures/girl.jpg", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	lightVAO_->Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	shader_->Use();
	shader_->SetInt("material.diffuse", 0);
	shader_->SetInt("material.specular", 1);
	shader_->SetInt("material.emission", 2);
	shader_->SetInt("spotLight.diffuse", 3);

}

void MultiLight::Update(const void* context) 
{
	DrawContext* c = (DrawContext*)context;
	glClear(GL_DEPTH_BUFFER_BIT);

	for (int i = 0;i < texturesCount_;i++) 
	{
		glActiveTexture(TextureNum(i));
		glBindTexture(GL_TEXTURE_2D, textures_[i]);
	}

	shader_->Use();
	DirectionLight dirLight;

	dirLight.direction = vec3(-0.2f, -1.0f, -0.3f);
	dirLight.ambient = normal * vec3(0.05f);
	dirLight.diffuse = normal * vec3(0.04f);
	dirLight.specular = normal * vec3(0.05f);

	SetDirectionLight(dirLight);

	SpotLight spotLight;
	spotLight.position = camera_->position;
	spotLight.direction = camera_->front;
	spotLight.color = normal;
	spotLight.ambient = normal * vec3(0.0f);
	spotLight.diffuse = normal * vec3(1.0f);
	spotLight.specular = normal * vec3(1.0f);
	spotLight.innterCutOff = glm::cos(glm::radians(12.5f));
	spotLight.outerCutOff = glm::cos(glm::radians(15.0f));
	spotLight.k_constant = 1.0f;
	spotLight.k_linear = 0.09f;
	spotLight.k_quadratic = 0.032f;
	SetSpotLight(spotLight);

	PointLight pointLights[NR_POINTER_LIGHTS];

	for (int i = 0;i < NR_POINTER_LIGHTS;i++) 
	{
		pointLights[i].position = pointLightPositions_[i];
		pointLights[i].color = vec3(glm::sin(2 * i + 60), glm::cos(2 * i + 45), glm::sin(3 * i + 30));
		pointLights[i].ambient = pointLights[i].color * vec3(0.05f);
		pointLights[i].diffuse = pointLights[i].color * vec3(0.2f);
		pointLights[i].specular = pointLights[i].color * vec3(0.2f);
		//pointLights[i].specular = pointLights[i].color * vec3(1.0f);
		pointLights[i].k_constant = 1.0f;
		pointLights[i].k_linear = 0.09f;
		pointLights[i].k_quadratic = 0.032f;
	}

	SetPointLight(pointLights, NR_POINTER_LIGHTS);

	shader_->SetFloat("material.diffuse", 0);
	shader_->SetFloat("material.specular", 1);
	shader_->SetFloat("material.emission", 2);
	shader_->SetFloat("material.shininess", 0.5f);

	mat4 view = camera_->GetViewMatrix();
	mat4 projection = camera_->GetProjectionMatrix(c->aspectRatio_);

	shader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));
	shader_->Set3fv("viewPos", VALUE_PTR(camera_->position));
	shader_->Set2f("emissionOffset", 0.0f, sin(c->time_) / 2);

	vao_->Bind();
	for (int i = 0;i < 10;i++)
	{
		mat4 model = mat4(1.0f);
		mat3 normalMat = mat3(1.0f);

		float angle = 20.0f * i;
		model = glm::translate(model, cubePositions_[i]);
		//model = glm::rotate(model, glm::radians(angle) * c->time_, vec3(1.0f, 0.3f, 0.5f));

		normalMat = glm::transpose(glm::inverse(model));
		shader_->SetMatrix4fv("model", 1, GL_FALSE, VALUE_PTR(model));
		shader_->SetMatrix3fv("normalMatrix", 1, GL_FALSE, VALUE_PTR(normalMat));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	lightVAO_->Bind();
	lightShader_->Use();
	for (int i = 0;i < NR_POINTER_LIGHTS;i++) 
	{
		mat4 lightMat = mat4(1.0f);
		lightMat = glm::translate(lightMat, pointLightPositions_[i]);
		lightMat = glm::scale(lightMat, vec3(0.2f));

		lightShader_->Use();
		lightShader_->SetMatrix4fv("model", 1, GL_FALSE, VALUE_PTR(lightMat));
		lightShader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));
		lightShader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
		lightShader_->Set3fv("lightColor",VALUE_PTR(pointLights[i].color));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

}

void MultiLight::SetPointLight(PointLight lights[], int length) 
{
	std::string key;
	std::string prefix;

	key.reserve(128);
	prefix.reserve(32);

	for (int i = 0;i < length;i++) 
	{
		prefix.clear();
		prefix = prefix.append("pointLights[").append(std::to_string(i)).append("].");

		key = key.append(prefix).append("position");
		shader_->Set3fv(key.c_str(), &lights[i].position[0]);
		key.clear();

		key = key.append(prefix).append("diffuse");
		shader_->Set3fv(key.c_str(), &lights[i].diffuse[0]);
		key.clear();

		key = key.append(prefix).append("ambient");
		shader_->Set3fv(key.c_str(), &lights[i].ambient[0]);
		key.clear();

		key = key.append(prefix).append("specular");
		shader_->Set3fv(key.c_str(), &lights[i].specular[0]);
		key.clear();

		key = key.append(prefix).append("k_constant");
		shader_->SetFloat(key.c_str(), lights[i].k_constant);
		key.clear();

		key = key.append(prefix).append("k_linear");
		shader_->SetFloat(key.c_str(), lights[i].k_constant);
		key.clear();

		key = key.append(prefix).append("k_quadratic");
		shader_->SetFloat(key.c_str(), lights[i].k_constant);
		key.clear();
	}
}

void MultiLight::SetDirectionLight(DirectionLight light) 
{
	shader_->Set3fv("dirLight.direction", &light.direction[0]);
	shader_->Set3fv("dirLight.diffuse", &light.diffuse[0]);
	shader_->Set3fv("dirLight.ambient", &light.ambient[0]);
	shader_->Set3fv("dirLight.specular", &light.specular[0]);
}

void MultiLight::SetSpotLight(SpotLight light) 
{
	shader_->Set3fv("spotLight.position", VALUE_PTR(light.position));
	shader_->Set3fv("spotLight.direction", VALUE_PTR(light.direction));
	shader_->Set3fv("spotLight.ambient", VALUE_PTR(light.ambient));
	shader_->Set3fv("spotLight.diffuse", VALUE_PTR(light.diffuse));
	shader_->Set3fv("spotLight.specular", VALUE_PTR(light.specular));

	shader_->SetFloat("spotLight.constant", light.k_constant);
	shader_->SetFloat("spotLight.linear", light.k_linear);
	shader_->SetFloat("spotLight.quadratic", light.k_quadratic);

	shader_->SetFloat("spotLight.innterCutOff", light.innterCutOff);
	shader_->SetFloat("spotLight.outerCutOff", light.outerCutOff);
}

void RenderModel::InputDrive(GLFWwindow* window, float time, float delaTime) 
{
	float speed = cameraSpeed_ * delaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::FORWARD, delaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::BACKWARD, delaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::RIGHT, delaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::LEFT, delaTime);
}

void RenderModel::MoveCamera(float xpos, float ypos, bool constrainPitch) 
{
	if (firstMouse_) 
	{
		lastX_ = xpos;
		lastY_ = ypos;
		firstMouse_ = false;
	}
	float xoffset = xpos - lastX_;
	float yoffset = lastY_ - ypos;

	lastX_ = xpos;
	lastY_ = ypos;

	camera_->ProcessMouseMovement(xoffset, yoffset, constrainPitch);
}

RenderModel::RenderModel(const char* modelPath, Shader* shader, FPS_Camera* camera) :DrawBase(shader, camera)
{
	model_ = new Model(modelPath);
}

RenderModel::~RenderModel() 
{
	delete model_;
}

void RenderModel::Init(const void* context) 
{

}

void RenderModel::Update(const void* context) 
{
	DrawContext* c = (DrawContext*)context;

	glClear(GL_DEPTH_BUFFER_BIT);

	shader_->Use();
	mat4 projection = camera_->GetProjectionMatrix(c->aspectRatio_, 0.1f, 100.0f);
	mat4 view = camera_->GetViewMatrix();
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));
	shader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
	mat4 model = mat4(1.0f);
	model = glm::translate(model, zero);
	model = glm::scale(model, normal);

	shader_->SetMatrix4fv("model", 1, GL_FALSE, VALUE_PTR(model));
	
	model_->Draw(*shader_);
}