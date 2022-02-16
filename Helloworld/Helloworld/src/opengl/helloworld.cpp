#include <opengl/helloworld.h>
#include<opengl/helperFun.h>
#include<stb/stb_image.h>
#include<windows/window.h>

bool LoadImage2D(uint* textureID, const char* path, GLint wrapST_param, GLint filter_param)
{

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
		//位置				//颜色
		-0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f,// left  
		0.1f, -0.1f, 0.0f, 0.0f, 1.0f, 0.0f, // right 
		0.0f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f,// top   
	};

	//usage:表示的是缓存区会被openGL如何使用，有三个参数：
	//GL_STATIC_DRAW:表示该缓冲区不会被修改
	//GL_DYNAMIC_DRAW:表示该缓冲区会被周期性修改
	//GL_STREAM_DRAW:表示该缓冲区会被频繁修改
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//参考文档<https://learnopengl-cn.github.io/01%20Getting%20started/05%20Shaders/#_5>
	//简单来说就是传递进去的数组只是一组一维的数据，而实际情况下，这数据是有多重含义的，所以就需要去指定解释各个数据的含义
	//设置位置信息
	//起始位置为0，每3个数据为一组数据，第一组的开始是从0开始，往后偏移6个float长度的数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//设置颜色信息
	//起始位置为0，每3个数据为一组数据，第一组的开始是从3个float之后的位置，往后偏移6个float长度的数据
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//先设置为默认值,由于后面在Draw有重新进行绑定，所以其实可以不用，但是为了严谨这里还是进行一次的设置
	//通过绑定0，来将当前的缓冲区设为默认值
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//通过绑定0，来将定点数组对象设为默认值
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

		////顶点位置				//颜色				// 纹理坐标
		//0.5f, 0.5f, 0.0f,		1.0f,0.0f,0.0f,		2.0f, 2.0f,		0.3f, 0.3f,// 右上角
		//0.5f, -0.5f, 0.0f,		0.0f,1.0f,0.0f,		2.0f, 0.0f,		0.3f, 0.0f,// 右下角
		//-0.5f, -0.5f, 0.0f,		0.0f,0.0f,1.0f,		0.0f, 0.0f,		0.0f, 0.0f,// 左下角
		//-0.5f, 0.5f, 0.0f,		1.0f,1.0f,0.0f,		0.0f, 2.0f,		0.0f, 0.1f,// 左上角

		//顶点位置				//颜色				// 纹理坐标
		0.5f, 0.5f, 0.0f,		1.0f,0.0f,0.0f,		2.0f, 2.0f,		1.0f, 1.0f,// 右上角
		0.5f, -0.5f, 0.0f,		0.0f,1.0f,0.0f,		2.0f, 0.0f,		1.0f, 0.0f,// 右下角
		-0.5f, -0.5f, 0.0f,		0.0f,0.0f,1.0f,		0.0f, 0.0f,		0.0f, 0.0f,// 左下角
		-0.5f, 0.5f, 0.0f,		1.0f,1.0f,0.0f,		0.0f, 2.0f,		0.0f, 1.0f,// 左上角
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
	//vbo_->SetBufferData(size, vertices, GL_STATIC_DRAW);
	ebo_->Bind();

	//两个三角形的顶点位置
	uint indices[] = {
		0,1,3, //第一个三角形
		1,2,3, //第二个三角形
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

	//绑定纹理，告诉openGL,这个纹理是一个2d的纹理，PS:2d纹理的坐标为s t
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//加载纹理图片
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	uchar* image = stbi_load("./assets/textures/container.jpg", &width, &height, &nrChannels, 0);

	if (image) 
	{
		//在当前的绑定纹理对象上在活动纹理单元生成纹理。
		//glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
		//target: 指定纹理的目标类型，GL_TEXTURE_1D,GL_TEDTURE_2D,GL_TEXTURE_3D
		//level:  指定多级渐远纹理的级别。0为基本图像级别。n表示第n个mipmap的缩小图片
		//internalFormat: 指定纹理储存的格式，例如突变只有RGB就存储为RGB值。
		//width:  指定纹理的宽度
		//height: 指定纹理图片的高度，或者纹理数组中的层数
		//border: 必须为0，api的历史遗留问题
		//format: 指定原图的数据格式，例如原图只有RGB,那这里就是RGB,如果对于有alpha通道切需要的png图片这里采用GL_RGBA	
		//type:   指定原图的数据类型
		//data:	  图片数据指针
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		//生成纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	//将纹理数据设置好后，记得要释放
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

	//将纹理数据设置好后，记得要释放
	stbi_image_free(image);

	shader_->Use();
	shader_->SetInt("ourTexture1", 0);
	shader_->SetInt("ourTexture2", 1);
}

void DrawRectangle::Draw(const void* context)
{

	//如果有多个纹理，需要对openGL进行定义，告诉是哪个的纹理单元。如果就一个纹理则不需要。openGL可以声明16个,0-15
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2_);

	shader_->Use();
	//shader_->SetInt("ourTexture1", 0);
	//shader_->SetInt("ourTexture2", 1);

	shader_->Set2f("towards", -1.0f, -1.0f);
	DrawContext* c= (DrawContext*)context;
	if (glfwGetKey(c->window_, GLFW_KEY_UP) == GLFW_PRESS) {
		visibilityValue_ += 0.1f;
	}
	if (glfwGetKey(c->window_, GLFW_KEY_DOWN) == GLFW_PRESS) {
		visibilityValue_ -= 0.1f;
	}

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

DrawTwoConnectTriangle::DrawTwoConnectTriangle(Shader* shader):DrawBase(shader)
{
	vao_ = new VAOContext();
	vbo_ = new VBOContext();
}

DrawTwoConnectTriangle::~DrawTwoConnectTriangle() 
{
	//模式选择，填充(默认，这里就是重置)
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

	//模式选择线框
	//该函数用于配置OpenGL如何绘制图元
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
		//位置				//颜色
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

void DrawDynamicTriangle::Draw(const void* context)
{
	vao_->Bind();
	vbo_->Bind();

	float vertices[18] = {
		//位置				//颜色
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


	//OpenGL 通过glEnable 与 glDisable 来控制开启或关闭一些功能，直到调用对应的功能来关闭为止
	//这里选择开启深度测试
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

	//绑定纹理
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
	
	DrawContext* c = (DrawContext*)context;

	// document: < https://learnopengl-cn.github.io/01%20Getting%20started/08%20Coordinate%20Systems/#_2 >
	////创建一个模型矩阵
	//mat4 model = mat4(1.0f);
	//model = glm::rotate(model, glm::radians(50.0f)*time, vec3(0.5f, 1.0f, 0.0f));

	////创建一个观察矩阵
	mat4 view = mat4(1.0f);
	//矩阵要向模型矩阵位移，可以认为是一个相机
	view = glm::translate(view, vec3(0.0f, 0.0f, -3.0f));

	//定义一个透视举证，使用透视投影
	mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), c->windowWidth_ / c->windowHeight_, 0.01f, 100.0f);

	//shader_->SetMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
	shader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(view));
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));

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
		mat4 model;
		model = glm::translate(model, cubePositions_[i]);
		float angle = 0.0f;
		angle = 20.0f * (i + 1);
		model = glm::rotate(model, glm::radians(angle)*c->time_, vec3(1.0f, 0.3f, 0.5f));
		shader_->SetMatrix4fv("model",1,GL_FALSE,&model[0][0]);


		//在OpenGL中，渲染一个物体中所有的顶点都要经过一下的空间坐标转换:局部坐标->世界坐标->观察坐标->裁剪坐标->屏幕坐标的过程，这些的过程都可以通过矩阵的变化来实现转换
		//在OpenGL中，我们提供的坐标矩阵可到裁剪坐标即可，OpenGL会帮助我们通过使用glViewPort来标准化设备坐标到屏幕坐标
		//裁剪坐标即：透视矩阵(裁剪空间) * 观察矩阵(观察空间) * 世界矩阵(世界空间) * 局部坐标(局部空间)  V clip = M projection * M view * M model * M local (一定要从右往左) 矩阵不符合交换律
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
	
	//OpenGL 通过glEnable 与 glDisable 来控制开启或关闭一些功能，直到调用对应的功能来关闭为止
	//这里选择开启深度测试
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

	//绑定纹理
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

	DrawContext* c = (DrawContext*)context;

	// document: < https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/#_1 >
	////创建一个模型矩阵
	//mat4 model = mat4(1.0f);
	//model = glm::rotate(model, glm::radians(50.0f)*time, vec3(0.5f, 1.0f, 0.0f));
	
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

	mat4 view = mat4(1.0f);
	//lookAt(eye,center,up)
	//eye: 相机位置
	//center:相机指向的目标位置
	//up:世界坐标中的向上向量
	//计算出来的矩阵就可以用来作为观察矩阵
	view = glm::lookAt(cameraPos_, cameraPos_ + cameraFront_, cameraUp_);
	mat4 v = camera_->GetViewMatrix();
	//std::cout << "观察举证 view: \n{\n " << view[0][0] << "," << view[0][1] << "," << view[0][2] << "," << view[0][3] << ",\n"
	//	<< view[1][0] << "," << view[1][1] << "," << view[1][2] << "," << view[1][3] << ",\n" << view[2][0] << "," << view[2][1] << "," << view[2][2] << "," << view[2][3]
	//	<< "\n" << view[3][0] << "," << view[3][1] << "," << view[3][2] << "," << view[3][3] << "\n}";

	mat4 projection = mat4(1.0f);
	projection = camera_->GetProjectionMatrix(c->windowWidth_ / c->windowHeight_);

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
	Window* w = (Window*)context;

	//w->RegisterMousePosition(([](GLFWwindow* window, double xpos, double ypos) {Mouse_Callback(window, xpos, ypos);}));

	//OpenGL 通过glEnable 与 glDisable 来控制开启或关闭一些功能，直到调用对应的功能来关闭为止
	//这里选择开启深度测试
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

void DrawSphere::Draw(const void* context)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_->Use();

	DrawContext* c = (DrawContext*)context;

	// document: < https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/#_1 >
	////创建一个模型矩阵
	//mat4 model = mat4(1.0f);
	//model = glm::rotate(model, glm::radians(50.0f)*time, vec3(0.5f, 1.0f, 0.0f));

	float speed = cameraSpeed_ * c->delaTime_;

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

	mat4 view = mat4(1.0f);
	//lookAt(eye,center,up)
	//eye: 相机位置
	//center:相机指向的目标位置
	//up:世界坐标中的向上向量
	//计算出来的矩阵就可以用来作为观察矩阵
	view = glm::lookAt(cameraPos_, cameraPos_ + cameraFront_, cameraUp_);
	mat4 v = camera_->GetViewMatrix();
	//std::cout << "观察举证 view: \n{\n " << view[0][0] << "," << view[0][1] << "," << view[0][2] << "," << view[0][3] << ",\n"
	//	<< view[1][0] << "," << view[1][1] << "," << view[1][2] << "," << view[1][3] << ",\n" << view[2][0] << "," << view[2][1] << "," << view[2][2] << "," << view[2][3]
	//	<< "\n" << view[3][0] << "," << view[3][1] << "," << view[3][2] << "," << view[3][3] << "\n}";

	mat4 projection = mat4(1.0f);
	projection = camera_->GetProjectionMatrix(c->windowWidth_ / c->windowHeight_);

	shader_->SetMatrix4fv("view", 1, GL_FALSE, VALUE_PTR(v));
	shader_->SetMatrix4fv("projection", 1, GL_FALSE, VALUE_PTR(projection));

	//shader_->Set2f("towards", -1.0f, -1.0f);

	shader_->SetFloat("visibility", Clamp01(visibilityValue_));

	vao_->Bind();

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

	//更改绑定，绑定光照的VAO
	lightVAO_->Bind();
	vbo_->Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
}

void LightDraw::Draw(const void* context) 
{

	DrawContext* c = (DrawContext*)context;
	ProcessInput(c);

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

	//两个3D正方体的共用相同的观察、透视矩阵、model自己重新设置
	mat4 model = mat4(1.0f);
	model = glm::rotate(model, glm::radians(c->time_ * 20.0f), vec3(0.0f, 1.0f, 0.0f));
	mat3 normalMatrix = mat3(1.0);
	//法线矩阵在CPU这里进行计算 G = (M^-1)^T
	normalMatrix = glm::transpose(glm::inverse(model));
	model = glm::scale(model, vec3(5.0f, 0.2f, 5.0f));
	mat4 projection = camera_->GetProjectionMatrix(c->windowWidth_ / c->windowHeight_);
	mat4 view = camera_->GetViewMatrix();

	vec3 diffuseColor = lightColor * vec3(0.5f);
	vec3 ambientColor = diffuseColor * vec3(0.2f);

	//第一个绘制第一个vao
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

	shader_->Set3f("material.specular", 0.5f,0.5f,0.5f);//设置镜面强度
	shader_->Set3f("material.diffuse", 1.0f, 0.5f, 0.31f);
	shader_->Set3f("material.ambient", 1.0f, 0.5f, 0.31f);
	shader_->SetFloat("material.shininess", 32.0f);//设置反射度

	vao_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//绘制光源，用第二个vao
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

LightingMapsDraw::LightingMapsDraw(Shader* shader, Shader* lightShader, FPS_Camera* camera) :DrawBase(shader, camera)
{
	lightShader_ = lightShader;
	vao_ = new VAOContext();
	vbo_ = new VBOContext();
	lightVAO_ = new VAOContext();

	glGenTextures(1, &diffuseTex_);
	glGenTextures(1, &specularTex_);
	glGenTextures(1, &emissionTex_);
}

LightingMapsDraw::~LightingMapsDraw()
{
	glDeleteTextures(1, &diffuseTex_);
	glDeleteTextures(1, &specularTex_);
	glDeleteBuffers(1, &emissionTex_);

	delete vao_;
	delete vbo_;
	delete lightVAO_;
}

void LightingMapsDraw::Init(const void* context)
{
	
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	vao_->Bind();
	vbo_->Bind();

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	bool success = LoadImage2D(&diffuseTex_, "./assets/textures/container2.png", GL_REPEAT, GL_NEAREST);
	if (!success) return;

	success = LoadImage2D(&specularTex_, "./assets/textures/container2_specular.png", GL_REPEAT, GL_NEAREST);
	//练习3
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

void LightingMapsDraw::Draw(const void* context)
{

	DrawContext* c = (DrawContext*)context;
	ProcessInput(c);

	vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	//lightPos.x = 1.0f + sin(c->time_) * 2.0f;
	//lightPos.y = sin(c->time_/2) * 1.0f;

	glClear(GL_DEPTH_BUFFER_BIT);

	mat4 model = mat4(1.0f);
	mat3 normalMatrix = mat3(1.0);
	normalMatrix = glm::transpose(glm::inverse(model));
	model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f) * sin(c->time_ / 2) * 1.0f);
	model = glm::rotate(model, glm::radians(c->time_ * 10.0f), vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, vec3(5.0f, 0.2f, 5.0f));
	mat4 projection = camera_->GetProjectionMatrix(c->windowWidth_ / c->windowHeight_);
	mat4 view = camera_->GetViewMatrix();

	vec3 diffuseColor = lightColor * vec3(0.5f);
	vec3 ambientColor = lightColor * vec3(2.0f);
	vec3 ambient = lightColor * vec3(0.5f);

	//第一个绘制第一个vao
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

	//绘制光源，用第二个vao
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

void LightingMapsDraw::MoveCamera(float xpos, float ypos, bool constrainPitch)
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

void LightingMapsDraw::ProcessInput(DrawContext* c)
{

	float speed = cameraSpeed_ * c->delaTime_;

	if (glfwGetKey(c->window_, GLFW_KEY_W) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::FORWARD, c->delaTime_);
	}
	if (glfwGetKey(c->window_, GLFW_KEY_S) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::BACKWARD, c->delaTime_);
	}
	if (glfwGetKey(c->window_, GLFW_KEY_D) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::RIGHT, c->delaTime_);
	}
	if (glfwGetKey(c->window_, GLFW_KEY_A) == GLFW_PRESS) {
		camera_->ProcessKeyboard(FPS_Camera::ECameraMovement::LEFT, c->delaTime_);
	}

}