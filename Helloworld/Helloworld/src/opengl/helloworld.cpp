#include <opengl/helloworld.h>
#include<opengl/helperFun.h>
#include<stb/stb_image.h>
#include<windows/window.h>

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

	vbo_->SetBufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);

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

void DrawTriangle::Draw(const void* context)
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

void DrawRectangle::Draw(const void* context)
{

	//����ж��������Ҫ��openGL���ж��壬�������ĸ�������Ԫ�������һ����������Ҫ��openGL��������16��,0-15
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2_);

	shader_->Use();
	//shader_->SetInt("ourTexture1", 0);
	//shader_->SetInt("ourTexture2", 1);

	float time = glfwGetTime();

	shader_->Set2f("towards", -1.0f, -1.0f);
	DrawContext* c= (DrawContext*)context;
	if (glfwGetKey(c->window_, GLFW_KEY_UP) == GLFW_PRESS) {
		visibilityValue_ += 0.1f;
	}
	if (glfwGetKey(c->window_, GLFW_KEY_DOWN) == GLFW_PRESS) {
		visibilityValue_ -= 0.1f;
	}

	shader_->SetFloat("visibility", Clamp01(visibilityValue_));
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));

	trans = glm::rotate(trans, time, glm::vec3(0, 0, 1.0f));
	shader_->SetMatrix4fv("transform", 1, GL_FALSE, glm::value_ptr(trans));

	vao_->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
	float value = sin(time);
	trans = glm::scale(trans, glm::vec3(1.0f,1.0f, 1.0f) * Abs(value));
	trans = glm::rotate(trans, time, glm::vec3(0.0f, 1.0f, 0.0f));
	shader_->SetMatrix4fv("transform", 1, GL_FALSE, glm::value_ptr(trans));

	vao_->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void DrawTwoConnectTriangle::Draw(const void* context)
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

void DrawTwoTriangleUseDiffVAOandVBO::Draw(const void* context)
{
	shader_->Use();
	glBindVertexArray(vaos_[0]);

	shader_->SetBoolean("useOffset", true);
	float time = glfwGetTime();
	float valueX = sin(time);
	float valueY = cos(time);
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

void DrawDynamicTriangle::Draw(const void* context)
{
	vao_->Bind();
	vbo_->Bind();

	float vertices[18] = {
		//λ��				//��ɫ
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// left  
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right 
			0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,// top  
	};
	
	float time = glfwGetTime();
	float cosValue = cos(time);
	float sinValue = sin(time);

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

	float valueX = sin(time);
	float valueY = cos(time);

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

void Draw3D::Draw(const void* context)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2_);

	shader_->Use();
	float time = glfwGetTime();
	DrawContext* c = (DrawContext*)context;

	// document: < https://learnopengl-cn.github.io/01%20Getting%20started/08%20Coordinate%20Systems/#_2 >
	////����һ��ģ�;���
	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(50.0f)*time, glm::vec3(0.5f, 1.0f, 0.0f));

	////����һ���۲����
	glm::mat4 view = glm::mat4(1.0f);
	//����Ҫ��ģ�;���λ�ƣ�������Ϊ��һ�����
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	//����һ��͸�Ӿ�֤��ʹ��͸��ͶӰ
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), c->windowWidth_ / c->windowHeight_, 0.01f, 100.0f);

	//shader_->SetMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
	shader_->SetMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));

	//shader_->Set2f("towards", -1.0f, -1.0f);

	if (glfwGetKey(c->window_, GLFW_KEY_UP) == GLFW_PRESS) {
		visibilityValue_ += 0.1f;
	}
	if (glfwGetKey(c->window_, GLFW_KEY_DOWN) == GLFW_PRESS) {
		visibilityValue_ -= 0.1f;
	}

	shader_->SetFloat("visibility", Clamp01(visibilityValue_));

	vao_->Bind();

	for (uint i = 0; i < 10; i++) 
	{
		glm::mat4 model;
		model = glm::translate(model, cubePositions_[i]);
		float angle = 0.0f;
		angle = 20.0f * (i + 1);
		model = glm::rotate(model, glm::radians(angle)*time, glm::vec3(1.0f, 0.3f, 0.5f));
		shader_->SetMatrix4fv("model",1,GL_FALSE,&model[0][0]);


		//��OpenGL�У���Ⱦһ�����������еĶ��㶼Ҫ����һ�µĿռ�����ת��:�ֲ�����->��������->�۲�����->�ü�����->��Ļ����Ĺ��̣���Щ�Ĺ��̶�����ͨ������ı仯��ʵ��ת��
		//��OpenGL�У������ṩ���������ɵ��ü����꼴�ɣ�OpenGL���������ͨ��ʹ��glViewPort����׼���豸���굽��Ļ����
		//�ü����꼴��͸�Ӿ���(�ü��ռ�) * �۲����(�۲�ռ�) * �������(����ռ�) * �ֲ�����(�ֲ��ռ�)  V clip = M projection * M view * M model * M local (һ��Ҫ��������) ���󲻷��Ͻ�����
		glDrawArrays(GL_TRIANGLES, 0, 36);
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
	Window* w = (Window*)context;

	//w->RegisterMousePosition(([](GLFWwindow* window, double xpos, double ypos) {Mouse_Callback(window, xpos, ypos);}));
	
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

void DrawCamera::Draw(const void* context)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2_);

	shader_->Use();
	float time = glfwGetTime();
	DrawContext* c = (DrawContext*)context;

	// document: < https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/#_1 >
	////����һ��ģ�;���
	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(50.0f)*time, glm::vec3(0.5f, 1.0f, 0.0f));
	
	float speed = cameraSpeed_  * c->delaTime_;

	if (glfwGetKey(c->window_, GLFW_KEY_U) == GLFW_PRESS) {
		visibilityValue_ += 0.1f;
	}
	if (glfwGetKey(c->window_, GLFW_KEY_P) == GLFW_PRESS) {
		visibilityValue_ -= 0.1f;
	}
	if (glfwGetKey(c->window_, GLFW_KEY_W) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::FORWARD, c->delaTime_);
		//cameraPos_ += speed * cameraFront_;
	}
	if (glfwGetKey(c->window_, GLFW_KEY_S) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::BACKWARD, c->delaTime_);
		//cameraPos_ -= speed * cameraFront_;
	}
	if (glfwGetKey(c->window_, GLFW_KEY_D) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::RIGHT, c->delaTime_);
		//cameraPos_ += glm::normalize(glm::cross(cameraFront_, cameraUp_)) * speed;
	}
	if (glfwGetKey(c->window_, GLFW_KEY_A) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::LEFT, c->delaTime_);
		//cameraPos_ -= glm::normalize(glm::cross(cameraFront_, cameraUp_)) * speed;
	}

	glm::mat4 view = glm::mat4(1.0f);
	//lookAt(eye,center,up)
	//eye: ���λ��
	//center:���ָ���Ŀ��λ��
	//up:���������е���������
	//��������ľ���Ϳ���������Ϊ�۲����
	view = glm::lookAt(cameraPos_, cameraPos_ + cameraFront_, cameraUp_);
	glm::mat4 v = camera_->GetViewMatrix();
	//std::cout << "�۲��֤ view: \n{\n " << view[0][0] << "," << view[0][1] << "," << view[0][2] << "," << view[0][3] << ",\n"
	//	<< view[1][0] << "," << view[1][1] << "," << view[1][2] << "," << view[1][3] << ",\n" << view[2][0] << "," << view[2][1] << "," << view[2][2] << "," << view[2][3]
	//	<< "\n" << view[3][0] << "," << view[3][1] << "," << view[3][2] << "," << view[3][3] << "\n}";

	glm::mat4 projection = glm::mat4(1.0f);
	projection = camera_->GetProjectionMatrix(c->windowWidth_ / c->windowHeight_);

	shader_->SetMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(v));
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));

	//shader_->Set2f("towards", -1.0f, -1.0f);

	shader_->SetFloat("visibility", Clamp01(visibilityValue_));

	vao_->Bind();

	for (uint i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions_[i]);
		float angle = 0.0f;
		angle = 20.0f * (i + 1);
		model = glm::rotate(model, glm::radians(angle) * time, glm::vec3(1.0f, 0.3f, 0.5f));
		shader_->SetMatrix4fv("model", 1, GL_FALSE, &model[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
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

LightDraw::LightDraw(Shader* shader, Shader* lightShader,FPS_Camera* camera) :DrawBase(shader, camera)
{
	lightShader_ = lightShader;
	vao_ = new VAOContext();
	vbo_ = new VBOContext();
	lightVAO_ = new VAOContext();
}

LightDraw::~LightDraw() 
{
	delete vao_;
	delete vbo_;
	delete lightVAO_;
}

void LightDraw::Init(const void* context) 
{
	float vertices[] = {
		  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	vao_->Bind();
	vbo_->Bind();

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//���İ󶨣��󶨹��յ�VAO
	lightVAO_->Bind();
	vbo_->Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
}

void LightDraw::Draw(const void* context) 
{

	DrawContext* c = (DrawContext*)context;
	ProcessInput(c);

	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

	glClear(GL_DEPTH_BUFFER_BIT);

	//����3D������Ĺ�����ͬ�Ĺ۲졢͸�Ӿ���model�Լ���������
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(5.0f, 0.2f, 5.0f));
	glm::mat4 projection = camera_->GetProjectionMatrix(c->windowWidth_ / c->windowHeight_);
	glm::mat4 view = camera_->GetViewMatrix();

	//��һ�����Ƶ�һ��vao
	shader_->Use();
	shader_->Set3f("objectColor", 1.0f, 0.5f, 0.31f);
	shader_->Set3f("lightColor", 1.0f, 1.0f, 1.0f);
	shader_->Set3fv("lightPos", &lightPos[0]);
	shader_->SetMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
	shader_->SetMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
	shader_->Set3fv("viewPos", &camera_->position[0]);
	shader_->SetFloat("specularStrength", 0.5f);

	vao_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//���ƹ�Դ���õڶ���vao
	lightShader_->Use();

	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));

	lightShader_->SetMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
	lightShader_->SetMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
	lightShader_->SetMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
	lightVAO_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}


void LightDraw::MoveCamera(float xpos, float ypos, bool constrainPitch)
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

void LightDraw::ProcessInput(DrawContext* c) 
{

	float speed = cameraSpeed_ * c->delaTime_;

	if (glfwGetKey(c->window_, GLFW_KEY_W) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::FORWARD, c->delaTime_);
		//cameraPos_ += speed * cameraFront_;
	}
	if (glfwGetKey(c->window_, GLFW_KEY_S) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::BACKWARD, c->delaTime_);
		//cameraPos_ -= speed * cameraFront_;
	}
	if (glfwGetKey(c->window_, GLFW_KEY_D) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::RIGHT, c->delaTime_);
		//cameraPos_ += glm::normalize(glm::cross(cameraFront_, cameraUp_)) * speed;
	}
	if (glfwGetKey(c->window_, GLFW_KEY_A) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::LEFT, c->delaTime_);
		//cameraPos_ -= glm::normalize(glm::cross(cameraFront_, cameraUp_)) * speed;
	}

}