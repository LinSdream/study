#pragma once
#include<opengl/base.h>
#include<windows/window.h>
#include<functional>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;
typedef glm::mat3 mat3;

#define VALUE_PTR(value) glm::value_ptr(value)

bool LoadImage2D(uint* textureID, const char* path, GLint wrapST_param, GLint filter_param);

class HelloworldGradientEnvironment:public HelloworldEnvironment
{
public:
	void Background(GLFWwindow* window);
};

typedef std::function<void(Shader* shader)> DrawFun;

class DrawBase
{
public:

	DrawBase(Shader* shader,FPS_Camera* camera = NULL):camera_(camera) , shader_(shader){}
	virtual ~DrawBase() {}

	virtual void Init(const void* context) = 0;
	virtual void Draw(const void* context) = 0;

	virtual void MoveCamera(float xpos, float ypos, bool constrainPitch = true)
	{
		camera_->ProcessMouseMovement(xpos, ypos, constrainPitch);
	}

	virtual void CameraScroll(float yoffset) 
	{
		camera_->ProcessMouseScroll(yoffset);
	}

protected:

	Shader* shader_;
	FPS_Camera* camera_;

};

class Context
{
public:

	HelloworldEnvironment* env_;
	Window* windows_;
	DrawBase** draws_;
	ShadersManager* shaderManager_;
	FPS_Camera* camera_;

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
			if(draws_[i] != NULL)
				delete draws_[i];
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
	float time_;
};

class DrawTriangle:public DrawBase
{

public :

	DrawTriangle(Shader* shader);
	~DrawTriangle();

	void Draw(const void* context);
	void Init(const void* context);

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
	void Init(const void* context);

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
	void Init(const void* context);

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
	void Init(const void* context);

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
	void Init(const void* context);

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
	void Init(const void* context);

private:

	VAOContext* vao_;
	VBOContext* vbo_;
};

class DrawCamera :public DrawBase 
{

public:

	DrawCamera(Shader* shader,FPS_Camera* camera);
	~DrawCamera();

	void Draw(const void* context);
	void Init(const void* context);
	void MoveCamera(float xpos, float ypos, bool constrainPitch = true);

private:


	VAOContext* vao_;
	EBOContext* ebo_;
	VBOContext* vbo_;

	uint texture1_;
	uint texture2_;

	float visibilityValue_;
	bool firstMouse_;
	float lastX_;
	float lastY_;
	float yaw_;
	float pitch_;

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

class DrawSphere :public DrawBase
{

public:

	DrawSphere(Shader* shader, FPS_Camera* camera);
	~DrawSphere();

	void Draw(const void* context);
	void Init(const void* context);
	void MoveCamera(float xpos, float ypos, bool constrainPitch = true);

private:


	VAOContext* vao_;
	EBOContext* ebo_;
	VBOContext* vbo_;

	float visibilityValue_;
	bool firstMouse_;
	float lastX_;
	float lastY_;
	float yaw_;
	float pitch_;

	glm::vec3 cameraPos_ = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp_ = glm::vec3(0.0f, 1.0f, 0.0f);
	float cameraSpeed_ = 2.5f;
};


class LightDraw :public DrawBase
{
public:

	LightDraw(Shader* shader, Shader* lightShader,FPS_Camera* camera);
	~LightDraw();

	void Draw(const void* context);
	void Init(const void* context);
	void MoveCamera(float xpos, float ypos, bool constrainPitch = true);

private:

	Shader* lightShader_;
	VAOContext* vao_;
	VAOContext* lightVAO_;
	VBOContext* vbo_;

	bool firstMouse_;
	float lastX_;
	float lastY_;
	float yaw_;
	float pitch_;

	glm::vec3 cameraPos_ = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp_ = glm::vec3(0.0f, 1.0f, 0.0f);
	float cameraSpeed_ = 2.5f;

	void ProcessInput(DrawContext* c);
};

class LightingMapsDraw :public DrawBase 
{

public:
	LightingMapsDraw(Shader* shader, Shader* lightShader, FPS_Camera* camera);
	~LightingMapsDraw();

	void Draw(const void* context);
	void Init(const void* context);
	void MoveCamera(float xpos, float ypos, bool constrainPitch = true);

private:

	Shader* lightShader_;
	VAOContext* vao_;
	VAOContext* lightVAO_;
	VBOContext* vbo_;

	uint diffuseTex_;
	uint specularTex_;
	uint emissionTex_;

	bool firstMouse_;
	float lastX_;
	float lastY_;
	float yaw_;
	float pitch_;

	glm::vec3 cameraPos_ = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp_ = glm::vec3(0.0f, 1.0f, 0.0f);
	float cameraSpeed_ = 2.5f;

	void ProcessInput(DrawContext* c);
};
