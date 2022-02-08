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

	virtual void Init(const void* vertices, int size) = 0;
	virtual void Draw(const void* context) = 0;

protected:

	Shader* shader_;
};

class Context
{

public:
	HelloworldEnvironment* env_;
	DrawBase* draw1_;
	DrawBase* draw2_;
	ShadersManager* shaderManager_;
	float visibilityValue_;
	GLFWwindow* window_;
};


class DrawTriangle:public DrawBase
{

public :

	DrawTriangle(Shader* shader);
	~DrawTriangle();

	void Draw(const void* context);
	void Init(const void* vertices, int size);

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
	void Init(const void* vertices,int size);

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
	void Init(const void* vertices, int size);

private:

	VAOContext* vao_;
	EBOContext* ebo_;
	VBOContext* vbo_;

	uint texture1_;
	uint texture2_;
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
	void Init(const void* vertices,int size);

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