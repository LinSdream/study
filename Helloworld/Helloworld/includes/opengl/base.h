#pragma once
#include<opengl/helperFun.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<iostream>
#include<map>
#include <list>
#include<vector>
//
//namespace MySpace
//{
//
//	template<typename ReturnType,typename ...ParamType>
//	class IDelegate
//	{
//	public:
//		virtual ~IDelegate() {};
//		virtual bool IsType(const std::type_info& type) = 0;
//		virtual ReturnType Invoke(ParamType... params) = 0;
//		virtual bool Compare(IDelegate<ReturnType, ParamType...>* del) const = 0;
//	};
//
//	template<typename ReturnType, typename ...ParamType>
//	class StaticDelegate :public IDelegate<ReturnType, ParamType...>
//	{
//	public:
//		typedef ReturnType (*Func)(ParamType...);
//		StaticDelegate(Func func) :func_(func) {}
//		virtual bool IsType(const std::type_info& type) { return type == typeid(StaticDelegate<ReturnType, ParamType...>); }
//		virtual ReturnType Invoke(ParamType... params) { func_(params); }
//		virtual bool Compare(IDelegate<ReturnType,ParamType...>* del) const
//		{
//			if (del == NULL || !del->IsType(typeid(StaticDelegate<ReturnType,ParamType...>))) return false;
//			StaticDelegate<ReturnType, ParamType...>* cast = static_cast<StaticDelegate<ReturnType, ParamType...>*>(del);
//			return cast->func_ == func_;
//		}
//
//	private:
//		Func func_;
//	};
//
//	template<typename T, typename ReturnType, typename ...ParamType>
//	class MethodDelegate :public IDelegate<ReturnType, ParamType...>
//	{
//
//	public:
//		typedef ReturnType(T::* Method)(ParamType...);
//		MethodDelegate(T* _obj, Method _method) :obj_(_obj), method_(_method){};
//		virtual bool IsType(const std::type_info& type) { return typeid(MethodDelegate<T, ReturnType, ParamType...>) == type; };
//		virtual ReturnType Invoke(ParamType params) { (obj_->*method_)(params); }//->*method_; }
//		virtual bool Compare(IDelegate<ReturnType, ParamType...>* del) const
//		{
//			if (del == NULL || !del->IsType(typeid(MethodDelegate<T,ReturnType, ParamType...>))) return false;
//			MethodDelegate<T, ReturnType, ParamType...>* cast = static_cast<MethodDelegate<T, ReturnType, ParamType...>*>(del);
//			return cast->obj_ == obj_ && cast->method_ == method_;
//		}
//
//	private:
//		T* obj_;
//		Method method_;
//	};
//
//
//	template<typename T>
//	inline StaticDelegate<T>* NewDelegate(void(*func)())
//	{
//		return new StaticDelegate<T>(func);
//	}
//
//	template<typename T, typename K>
//	inline MethodDelegate<T,K>* NewDelegate(T* obj, void (T::* method_)())
//	{
//		return new MethodDelegate<T,K>(obj, method_);
//	}
//
//	template<typename ReturnType, typename ...ParamType>
//	class Delegate
//	{
//
//	public:
//
//		typedef std::list<IDelegate<ReturnType, ParamType...>*> ListDelegate;
//		typedef ListDelegate::iterator ListDelegateIter;
//		typedef ListDelegate::const_iterator ConstListDelegteIter;
//
//		Delegate() {};
//		~Delegate() { Clear(); }
//		bool Empty() const
//		{
//			for (ConstListDelegteIter iter = delegates_.begin();iter != delegates_.end();++iter)
//			{
//				if (*iter) return false;
//			}
//			return true;
//		}
//
//		void Clear()
//		{
//			for (ListDelegateIter iter = delegates_.begin();iter != delegates_.end();++iter)
//			{
//				if (*iter)
//				{
//					delete (*iter);
//					(*iter) = NULL;
//				}
//			}
//		}
//
//		Delegate<ReturnType,ParamType...>& operator+=(IDelegate<ReturnType,ParamType...>* del)
//		{
//			for (ListDelegateIter iter = delegates_.begin();iter != delegates_.end();++iter)
//			{
//				if ((*iter) && (*iter)->Compare(del))
//				{
//					delete del;
//					return *this;
//				}
//			}
//			delegates_.push_back(del);
//			return *this;
//		}
//
//		Delegate<ReturnType, ParamType>& operator-=(IDelegate<ReturnType, ParamType>* del)
//		{
//			for (ListDelegateIter iter = delegates_.begin();iter != delegates_.end();++iter)
//			{
//				if ((*iter) != del && (*iter)->Compare(del))
//				{
//					if ((*iter) != del) delete (*iter);
//					(*iter) = NULL;
//					break;
//				}
//			}
//
//			delete del;
//			return *this;
//		}
//
//		std::vector<ReturnType> operator()(ParamType... params)
//		{
//			ListDelegateIter iter = delegates_.begin();
//			std::vector<ReturnType> results;
//
//			while (iter != delegates_.end())
//			{
//				if ((*iter) == NULL)
//				{
//					iter = delegates_.erase(iter);
//				}
//				else
//				{
//					results.push_back((*iter)->Invoke());
//					++iter;
//				}
//			}
//			return results;
//		}
//
//
//	private:
//
//		//Delegate(const Delegate& _event);
//		//Delegate& operator = (const Delegate& _event);
//
//		ListDelegate delegates_;
//	};
//}

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
/// 索引缓冲对象(Element Buffer Object‘EBO’/Index Buffer Object 'IBO')
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

	void Use() const { glUseProgram(programID_); }
	uint GetID() const { return programID_; }
	int InitiationStatus() const { return code_; }

	void Set4f(const char* name, float x, float y, float z, float w);
	void Set4fv(const char* name, const float* value);
	void Set3f(const char* name, float x, float y, float z);
	void Set3fv(const char* name, const float* value);
	void Set2f(const char* name, float x, float y);
	void Set2fv(const char* name, const float* value);
	void SetFloat(const char* name, float value);
	void SetBoolean(const char* name, bool value);
	void SetInt(const char* name, int value);
	void SetMatrix4fv(const char* name,int matrixCount, GLboolean transpose, const float* value);
	void SetMatrix3fv(const char* name,int matrixCount, GLboolean transpose, const float* value);

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
	Shader* GetShader(std::string shaderName) const;
	bool RemoveShader(std::string shaderName);
	int GetShaderCount() const;
	bool NullOrEmpty() const;
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

///暂未验证
class QuickString 
{
public:

	QuickString(uint _capacity);
	QuickString(const QuickString& _s);
	QuickString();
	~QuickString();

	void ResetSize(uint _capacity);
	QuickString& Append(const char* _s);
	QuickString& Append(const QuickString& _s);

	uint Length() const { return length_; }
	uint Capacity() const { return capacity_; }

	QuickString& operator+(const char* _s);
	QuickString& operator-(const char* _s);

	QuickString& operator+(const QuickString& _s);
	QuickString& operator-(const QuickString& _s);

	friend std::ostream& operator <<(std::ostream& _o, QuickString& _s);

private:

	char* str_;
	uint capacity_;
	uint length_;
};