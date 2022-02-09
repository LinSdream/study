#pragma once
#include<opengl/base.h>
#include<functional>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

class HelloworldGradientEnvironment:public HelloworldEnvironment
{
public:
	void Background(GLFWwindow* window);
};

typedef std::function<void(Shader* shader)> DrawFun;

class DrawBase
{
public:

	DrawBase(Shader* shader) { shader_ = shader; }
	virtual ~DrawBase() {}

	virtual void Init() = 0;
	virtual void Draw(const void* context) = 0;

protected:

	Shader* shader_;
};

class Context
{
public:
	HelloworldEnvironment* env_;

	DrawBase** draws_;

	ShadersManager* shaderManager_;
	float visibilityValue_;

	void InitDraws(int count) 
	{
		draws_ = new DrawBase * [count];drawsCount_
			= count;
	}

	void RecycleDraws()
	{
		if (draws_ == NULL) return;
		for (int i = 0; i < drawsCount_; i++)
		{
			if(draws_[i] != NULL) delete draws_[i];
		}
		delete draws_;
	}

	int DrawsCount() { return drawsCount_; };

private:

	int drawsCount_;
};

struct DrawContext 
{
	GLFWwindow* window_;
	float windowHeight_;
	float windowWidth_;
	float delaTime_;
};

class DrawTriangle:public DrawBase
{

public :

	DrawTriangle(Shader* shader);
	~DrawTriangle();

	void Draw(const void* context);
	void Init();

private:

	VAOContext* vao_;
	VBOContext* vbo_;

};

class DrawRectangle :public DrawBase
{
public:

	DrawRectangle(Shader* shader);
	~DrawRectangle();

	void Draw(const void* context);
	void Init();

private:

	VAOContext* vao_;
	EBOContext* ebo_;
	VBOContext* vbo_;
	uint texture1_;
	uint texture2_;

	float visibilityValue_;
};

class Draw3D :public DrawBase 
{
public :

	Draw3D(Shader* shader);
	~Draw3D();

	void Draw(const void* context);
	void Init();

private:

	VAOContext* vao_;
	EBOContext* ebo_;
	VBOContext* vbo_;

	uint texture1_;
	uint texture2_;

	float visibilityValue_;

	glm::vec3 cubePositions_[10] = 
	{ 
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
};

class DrawTwoConnectTriangle :public DrawBase
{
public:

	DrawTwoConnectTriangle(Shader* shader);
	~DrawTwoConnectTriangle();
	
	void Draw(const void* context);
	void Init();

private:

	VAOContext* vao_;
	VBOContext* vbo_;

};

class DrawTwoTriangleUseDiffVAOandVBO :public DrawBase 
{
public:

	DrawTwoTriangleUseDiffVAOandVBO(Shader* shader);
	~DrawTwoTriangleUseDiffVAOandVBO();

	void Draw(const void* context);
	void Init();

private:

	uint vaos_[2];
	uint vbos_[2];
};

class DrawDynamicTriangle:public DrawBase
{
public:

	DrawDynamicTriangle(Shader* shader);
	~DrawDynamicTriangle();

	void Draw(const void* context);
	void Init();

private:

	VAOContext* vao_;
	VBOContext* vbo_;
};

class DrawCamera :public DrawBase 
{

public:

	DrawCamera(Shader* shader);
	~DrawCamera();

	void Draw(const void* context);
	void Init();

private:

	void Mouse_Callback(GLFWwindow* window, double posX, double posY);

	VAOContext* vao_;
	EBOContext* ebo_;
	VBOContext* vbo_;

	uint texture1_;
	uint texture2_;

	float visibilityValue_;
	bool firstMouse_;
	float lastX_;
	float lastY_;
	//¸©Ñö½Ç
	float pitch_;
	//Æ«º½½Ç
	float yaw_;
	//ÊÓÒ° field of view
	float fov_;

	glm::vec3 cameraPos_ = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp_ = glm::vec3(0.0f, 1.0f, 0.0f);
	float cameraSpeed_ = 2.5f;

	glm::vec3 cubePositions_[10] =
	{
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
};