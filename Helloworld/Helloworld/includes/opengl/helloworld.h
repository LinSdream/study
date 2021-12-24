#pragma once
#include<opengl/helperFun.h>

#define SUCCESS 0
#define UNKNOWN -1
#define FAILED -2
#define ARRAY_SIZE(vertices) (sizeof(vertices)/sizeof(float))

class HelloworldEnvironment
{

public :
	virtual void Background(GLFWwindow* window);
	virtual void PressInput(GLFWwindow* window);
};

class HellowworldGradientEnvironment:public HelloworldEnvironment
{
public:
	void Background(GLFWwindow* window);
};

class ShaderContext 
{
public:

	ShaderContext();
	bool Link(uint** shaders,int size);
	void UseProgram();
	bool CreateShader(uint* shader,GLenum type,int shaderSouceSize, char* shaderSource);

private:
	uint shaderProgram_;
};

class VBOContext
{

public:

	VBOContext();
	~VBOContext();
	void SetBufferData(GLsizeiptr size, const void* data, GLenum usage);
	void Bind();

private:
	uint vbo_;
};

class VAOContext 
{
public:
	VAOContext();
	~VAOContext();
	void Bind();
private:
	uint vao_;
};


/// <summary>
/// Ë÷Òý»º³å¶ÔÏó(Element Buffer Object¡®EBO¡¯/Index Buffer Object 'IBO')
/// </summary>
class EBOContext 
{
public:

	EBOContext();
	~EBOContext();

	void Bind();

private:

	uint ebo_;
};

class DrawBase
{
public:
	
	DrawBase() {}
	virtual ~DrawBase() {}

	virtual void Init() = 0;
	virtual void Draw() = 0;

};

class DrawTriangle:public DrawBase
{

public :

	DrawTriangle();
	~DrawTriangle();

	void Draw();
	void Init();

private:

	VAOContext* vao_;
	VBOContext* vbo_;

};

class DrawRectangle :public DrawBase
{
public:

	DrawRectangle();
	~DrawRectangle();

	void Draw();
	void Init();

private:

	VAOContext* vao_;
	EBOContext* ebo_;
	VBOContext* vbo_;
};

class DrawTwoConnectTriangle :public DrawBase
{
public:

	DrawTwoConnectTriangle();
	~DrawTwoConnectTriangle();
	
	void Draw();
	void Init();

private:

	VAOContext* vao_;
	VBOContext* vbo_;

};


class DrawTwoTriangleUseDiffVAOandVBO :public DrawBase 
{
public:

	DrawTwoTriangleUseDiffVAOandVBO();
	~DrawTwoTriangleUseDiffVAOandVBO();

	void Draw();
	void Init();

private:

	uint vaos_[2];
	uint vbos_[2];
};
