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
	virtual void Update(const void* context) = 0;

	virtual void MoveCamera(float xpos, float ypos, bool constrainPitch = true)
	{
		camera_->ProcessMouseMovement(xpos, ypos, constrainPitch);
	}

	virtual void CameraScroll(float yoffset) 
	{
		camera_->ProcessMouseScroll(yoffset);
	}

	virtual void InputDrive(GLFWwindow* window, float time, float deltaTime) {}

protected:

	Shader* shader_;
	FPS_Camera* camera_;

};

struct DrawContext
{
	int windowHeight_;
	int windowWidth_;
	float aspectRatio_;
	float delaTime_;
	float time_;
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
			if (draws_[i] != NULL)
				delete draws_[i];
		}
		delete draws_;
	}

	int DrawsCount() { return drawsCount_; };

private:

	int drawsCount_;
};

class LightDrawBase :public DrawBase
{
public:
	LightDrawBase(Shader* cubeShader, Shader* lightShader, FPS_Camera* camera);
	~LightDrawBase();

	virtual void Init(const void* context) = 0;
	virtual void Update(const void* context) = 0;
	virtual void MoveCamera(float xpos, float ypos, bool constrainPitch = true);
	virtual void InputDrive(GLFWwindow* window, float time, float deltaTime);


protected:

	const vec3 normalVec3 = vec3(1.0f, 1.0f, 1.0f);

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

};

class DrawTriangle:public DrawBase
{

public :

	DrawTriangle(Shader* shader);
	~DrawTriangle();

	void Update(const void* context);
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

	void Update(const void* context);
	void Init(const void* context);
	void InputDrive(GLFWwindow* window, float time, float deltaTime);

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

	void Update(const void* context);
	void Init(const void* context);
	void InputDrive(GLFWwindow* window,float time, float deltaTime);

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
	
	void Update(const void* context);
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

	void Update(const void* context);
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

	void Update(const void* context);
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

	void Update(const void* context);
	void Init(const void* context);
	void MoveCamera(float xpos, float ypos, bool constrainPitch = true);
	void InputDrive(GLFWwindow* window, float time, float deltaTime);

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

	void Update(const void* context);
	void Init(const void* context);
	void MoveCamera(float xpos, float ypos, bool constrainPitch = true);
	void InputDrive(GLFWwindow* window, float time, float deltaTime);

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


class LightDraw :public LightDrawBase
{
public:

	LightDraw(Shader* shader, Shader* lightShader,FPS_Camera* camera);
	~LightDraw();

	void Update(const void* context);
	void Init(const void* context);
};

class LightingMapsDraw :public LightDrawBase 
{

public:
	LightingMapsDraw(Shader* shader, Shader* lightShader, FPS_Camera* camera);
	~LightingMapsDraw();

	void Update(const void* context);
	void Init(const void* context);

protected:

	float vertices_[36 * 8] = {
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

	uint diffuseTex_;
	uint specularTex_;
	uint emissionTex_;
};

class Lightcasters_DirectionalLight_Draw :public LightingMapsDraw
{
public:

	Lightcasters_DirectionalLight_Draw(Shader* cubeShader, Shader* lightShader, FPS_Camera* camera);
	~Lightcasters_DirectionalLight_Draw();

	void Update(const void* context);
	void Init(const void* context);
	

protected:

	vec3 cubePositions_[10] =
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

private :

	struct Light
	{
		vec3 color;
		vec3 direction;
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};
};

class Lightcasters_PointLight_Draw :public Lightcasters_DirectionalLight_Draw 
{
public:

	Lightcasters_PointLight_Draw(Shader* cubeShader, Shader* lightShader, FPS_Camera* camera);
	~Lightcasters_PointLight_Draw();

	void Update(const void* context);

private:

	struct Light
	{
		vec3 color;
		vec3 position;
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;

		float k_constant;
		float k_linear;
		float k_quadratic;
	};

	void SetLightShaderValue(Light light);
};

class Lightcasters_Spotlight_Draw :public Lightcasters_DirectionalLight_Draw 
{
public:
	Lightcasters_Spotlight_Draw(Shader* cubeShader, Shader* lightShader, FPS_Camera* camera);
	~Lightcasters_Spotlight_Draw();

	void Update(const void* context);

private:
	struct Light
	{
		vec3 color;
		vec3 position;
		vec3 direction;
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;

		float cutOff;

		float k_constant;
		float k_linear;
		float k_quadratic;
	};
};