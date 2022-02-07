#pragma once
#include<opengl/base.h>
#include<functional>

class HelloworldGradientEnvironment:public HelloworldEnvironment
{
public:
	void Background(GLFWwindow* window);
};

class DrawBase
{
public:

	DrawBase(Shader* shader) { shader_ = shader; }
	virtual ~DrawBase() {}

	virtual void Init(const void* vertices, int size) = 0;
	virtual void Draw(std::function<void(Shader* shader)> fun) = 0;

protected:

	Shader* shader_;
};

class DrawTriangle:public DrawBase
{

public :

	DrawTriangle(Shader* shader);
	~DrawTriangle();

	void Draw(std::function<void(Shader* shader)> fun);
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

	void Draw(std::function<void(Shader* shader)> fun);
	void Init(const void* vertices,int size);

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
	
	void Draw(std::function<void(Shader* shader)> fun);
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

	void Draw(std::function<void(Shader* shader)> fun);
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

	void Draw(std::function<void(Shader* shader)> fun);
	void Init();

private:

	VAOContext* vao_;
	VBOContext* vbo_;
};