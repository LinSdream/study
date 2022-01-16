#pragma once
#include<opengl/helperFun.h>
#include<iostream>
#include<map>

class HelloworldEnvironment
{

public:
	virtual void Background(GLFWwindow* window);
	virtual void PressInput(GLFWwindow* window);
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


class Shader
{
public:

	enum FailedCode
	{
		CREATE_VERTEX_SHADER_FAILED = 11,
		CREATE_FRAGMENT_SHADER_FAILED = 12,
		LINKE_PROGRAME_FAILED = 13,
	};

	Shader(char* vertex, char* fragment);
	~Shader();

	void Use() { glUseProgram(programID_); }
	uint GetID() { return programID_; }
	int InitiationStatus() { return code_; }

	void Set4f(const char* name, float x, float y, float z, float w);
	void Set3f(const char* name, float x, float y, float z);
	void Set2f(const char* name, float x, float y);
	void SetFloat(const char* name, float value);
	void SetBoolean(const char* name, bool value);
	void SetInt(const char* name, int value);

private:

	uint programID_;
	int code_;
	uint vertexShader_;
	uint fragmentShader_;
};

typedef void (*DrawFun)(Shader* shader);

class DrawBase
{
public:

	DrawBase(Shader* shader) { shader_ = shader; }
	virtual ~DrawBase() {}

	virtual void Init() = 0;
	virtual void Draw(DrawFun fun) = 0;

protected:

	Shader* shader_;
};


typedef std::map<std::string, Shader*> sshaderMap;
class ShadersManager
{
public:

	ShadersManager();
	~ShadersManager();

	bool CreateShader(std::string shaderName, const char* vsPath, const char* fsPath);
	Shader* GetShader(std::string shaderName);
	bool RemoveShader(std::string shaderName);
	int GetShaderCount();
	bool NullOrEmpty();

	Shader* operator[](std::string shaderName);

private:
	sshaderMap* shaderMap_;
};