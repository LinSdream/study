#pragma once
#include<opengl/helperFun.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<iostream>
#include<map>
#include <list>

namespace MySpace
{
	class IDelegate
	{
	public:
		virtual ~IDelegate() {};
		virtual bool IsType(const std::type_info& type) = 0;
		virtual void Invoke() = 0;
		virtual bool Compare(IDelegate* del) const = 0;
	};

	class StaticDelegate :public IDelegate
	{
	public:
		typedef void (*Func)();
		StaticDelegate(Func func) :func_(func) {}
		virtual bool IsType(const std::type_info& type) { return type == typeid(StaticDelegate); }
		virtual void Invoke() { func_(); }
		virtual bool Compare(IDelegate* del) const
		{
			if (del == NULL || !del->IsType(typeid(StaticDelegate))) return false;
			StaticDelegate* cast = static_cast<StaticDelegate*>(del);
			return cast->func_ == func_;
		}

	private:
		Func func_;
	};

	template<class T>
	class MethodDelegate :public IDelegate
	{

	public:
		typedef void(T::* Method)();
		MethodDelegate(T* _obj, Method _method) :obj_(_obj), method_(_method){};
		virtual bool IsType(const std::type_info& type) { return typeid(MethodDelegate) == type; };
		virtual void Invoke() { (obj_->*method_)(); }//->*method_; }
		virtual bool Compare(IDelegate* del) const
		{
			if (del == NULL || !del->IsType(typeid(MethodDelegate<T>))) return false;
			MethodDelegate* cast = static_cast<MethodDelegate<T>*>(del);
			return cast->obj_ == obj_ && cast->method_ == method_;
		}

	private:
		T* obj_;
		Method method_;
	};

	inline IDelegate* NewDelegate(void(*func)())
	{
		return new StaticDelegate(func);
	}

	template<class T>
	inline IDelegate* NewDelegate(T* obj, void (T::* method_)())
	{
		return new MethodDelegate<T>(obj, method_);
	}


	class Delegate
	{

	public:

		typedef std::list<IDelegate*> ListDelegate;
		typedef ListDelegate::iterator ListDelegateIter;
		typedef ListDelegate::const_iterator ConstListDelegteIter;

		Delegate() {};
		~Delegate() { Clear(); }
		bool Empty() const
		{
			for (ConstListDelegteIter iter = delegates_.begin();iter != delegates_.end();++iter)
			{
				if (*iter) return false;
			}
			return true;
		}

		void Clear()
		{
			for (ListDelegateIter iter = delegates_.begin();iter != delegates_.end();++iter)
			{
				if (*iter)
				{
					delete (*iter);
					(*iter) = NULL;
				}
			}
		}
		Delegate& operator+=(IDelegate* del)
		{
			for (ListDelegateIter iter = delegates_.begin();iter != delegates_.end();++iter)
			{
				if ((*iter) && (*iter)->Compare(del))
				{
					delete del;
					return *this;
				}
			}
			delegates_.push_back(del);
			return *this;
		}


		Delegate& operator-=(IDelegate* del)
		{
			for (ListDelegateIter iter = delegates_.begin();iter != delegates_.end();++iter)
			{
				if ((*iter) != del && (*iter)->Compare(del))
				{
					if ((*iter) != del) delete (*iter);
					(*iter) = NULL;
					break;
				}
			}

			delete del;
			return *this;
		}
		void operator()()
		{
			ListDelegateIter iter = delegates_.begin();
			while (iter != delegates_.end())
			{
				if ((*iter) == NULL)
				{
					iter = delegates_.erase(iter);
				}
				else
				{
					(*iter)->Invoke();
					++iter;
				}
			}
		}


	private:

		//Delegate(const Delegate& _event);
		//Delegate& operator = (const Delegate& _event);

		ListDelegate delegates_;
	};
}

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
	void SetMatrix4fv(const char* name,int matrixCount, GLboolean transpose, const float* value);

private:

	uint programID_;
	int code_;
	uint vertexShader_;
	uint fragmentShader_;
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
	void Recycle(Shader* shader);

	Shader* operator[](std::string shaderName);

private:
	sshaderMap* shaderMap_;
};

class FPS_Camera
{
public:

	enum ECameraMovement 
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
	};

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	float speed;
	float mouseSensitivity;
	float zoom;

	FPS_Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = -90.0f, float _pitch = 0.0f, float _speed = 2.5f, float _zoom = 45.0f, float _mouseSensitivity = 0.1f);
	FPS_Camera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch,float _speed = 2.5f, float _mouseSensitivity = 0.1f);

	virtual glm::mat4 GetViewMatrix();
	virtual glm::mat4 GetProjectionMatrix(float aspectRatio, float zNear = 0.1f, float zFar = 100.0f);
	virtual glm::mat4 CalculateLookAtMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp);

	virtual void ProcessKeyboard(ECameraMovement direction, float deltaTime);
	virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch);
	virtual void ProcessMouseScroll(float yoffset);

protected:

	virtual void UpdateCameraVectors();

};