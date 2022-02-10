#include<opengl/base.h>

void HelloworldEnvironment::Background(GLFWwindow* window)
{

}

void HelloworldEnvironment::PressInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

VBOContext::VBOContext()
{
	glGenBuffers(1, &vbo_);
}

VBOContext::~VBOContext()
{
	glDeleteBuffers(1, &vbo_);
}

void VBOContext::SetBufferData(GLsizeiptr size, const void* data, GLenum usage)
{
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VBOContext::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
}

VAOContext::VAOContext()
{
	glGenVertexArrays(1, &vao_);
}

VAOContext::~VAOContext()
{
	glDeleteVertexArrays(1, &vao_);
}

void VAOContext::Bind()
{
	glBindVertexArray(vao_);
}

EBOContext::EBOContext()
{
	glGenBuffers(1, &ebo_);
}

EBOContext::~EBOContext()
{
	glDeleteBuffers(1, &ebo_);
}

void EBOContext::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
}


Shader::Shader(char* vertex, char* fragment)
{
	programID_ = glCreateProgram();

	code_ = SUCCESS;

	vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader_, 1, &vertex, NULL);
	glCompileShader(vertexShader_);
	int success;
	glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[512];
		glGetShaderInfoLog(vertexShader_, 512, NULL, log);
		std::cout << "Create GL_VERTEX_SHADER Failed.Code:" << success << "Shader Source:" << vertex << std::endl;
		code_ = CREATE_VERTEX_SHADER_FAILED;
		return;
	}

	fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_, 1, &fragment, NULL);
	glCompileShader(fragmentShader_);
	glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[512];
		glGetShaderInfoLog(fragmentShader_, 512, NULL, log);
		std::cout << "Create GL_FRAGMENT_SHADER Failed.Code:" << success << "Shader Source:" << fragment << std::endl;
		code_ = CREATE_VERTEX_SHADER_FAILED;
		return;
	}

	glAttachShader(programID_, vertexShader_);
	glAttachShader(programID_, fragmentShader_);

	glLinkProgram(programID_);

	glGetProgramiv(programID_, GL_LINK_STATUS, &success);
	if (!success)
	{
		char log[512];
		glGetProgramInfoLog(programID_, 512, NULL, log);
		std::cout << "Link Shader Program Failed.Code:" << success << "Info:" << log << std::endl;
		code_ = LINKE_PROGRAME_FAILED;
		return;
	}

	glDeleteShader(vertexShader_);
	glDeleteShader(fragmentShader_);

}

Shader::~Shader()
{
	glDeleteShader(programID_);
}

void Shader::Set4f(const char* name, float x, float y, float z, float w)
{
	int uniformID = glGetUniformLocation(programID_, name);
	glUniform4f(uniformID, x, y, z, w);
}

void Shader::Set3f(const char* name, float x, float y, float z)
{
	int uniformID = glGetUniformLocation(programID_, name);
	glUniform3f(uniformID, x, y, z);
}

void Shader::Set2f(const char* name, float x, float y)
{
	int uniformID = glGetUniformLocation(programID_, name);
	glUniform2f(uniformID, x, y);
}

void Shader::SetFloat(const char* name, float value)
{
	int uniformID = glGetUniformLocation(programID_, name);
	glUniform1f(uniformID, value);
}

void Shader::SetBoolean(const char* name, bool value)
{
	int uniformID = glGetUniformLocation(programID_, name);
	glUniform1i(uniformID, (int)value);
}

void Shader::SetInt(const char* name, int value)
{
	int uniformID = glGetUniformLocation(programID_, name);
	glUniform1i(uniformID, value);
}

void Shader::SetMatrix4fv(const char* name, int matrixCount, GLboolean transpose, const float* value)
{
	int uniformID = glGetUniformLocation(programID_, name);
	//matrixCount:参数表示要发送给OpenGL多少个的举证
	//trnaspose: 表示是否对矩阵进行置换（行列交换）
	glUniformMatrix4fv(uniformID, matrixCount, transpose, value);
}

ShadersManager::ShadersManager()
{
	shaderMap_ = NULL;
}

ShadersManager::~ShadersManager()
{
	if (shaderMap_ == NULL) return;

	if (shaderMap_->empty())
	{
		delete shaderMap_;
		return;
	}

	for (sshaderMap::iterator i = shaderMap_->begin(); i != shaderMap_->end(); i++)
	{
		if (i->second)	delete i->second;
	}

	shaderMap_->clear();
	delete shaderMap_;
}

bool ShadersManager::CreateShader(std::string shaderName, const char* vsPath, const char* fsPath)
{

	if (shaderMap_ == NULL) shaderMap_ = new sshaderMap();

	sshaderMap::iterator iter = shaderMap_->find(shaderName);
	if (iter != shaderMap_->end()) return true;

	std::string vs = ReadFile(vsPath);
	if (vs.empty()) return false;
	std::string fs = ReadFile(fsPath);
	if (fs.empty()) return false;

	Shader* shader = new Shader((char*)vs.c_str(), (char*)fs.c_str());
	shaderMap_->insert(std::pair<std::string, Shader*>(shaderName, shader));
	return true;
}

bool ShadersManager::NullOrEmpty()
{
	if (shaderMap_ == NULL) return true;
	return shaderMap_->empty();
}

Shader* ShadersManager::GetShader(std::string shaderName)
{
	if (shaderMap_ == NULL) return NULL;
	if (shaderMap_->empty()) return NULL;
	sshaderMap::iterator iter = shaderMap_->find(shaderName);
	if (iter == shaderMap_->end()) return NULL;
	return shaderMap_->operator[](shaderName);
}

Shader* ShadersManager::operator[](std::string shaderName)
{
	return GetShader(shaderName);
}

int ShadersManager::GetShaderCount()
{
	if (shaderMap_ == NULL) return 0;
	return shaderMap_->size();
}

bool ShadersManager::RemoveShader(std::string shaderName)
{
	if (shaderMap_ == NULL) return false;
	if (shaderMap_->empty()) return false;
	sshaderMap::iterator iter = shaderMap_->find(shaderName);
	if (iter == shaderMap_->end()) return false;
	if (iter->second) delete iter->second;
	shaderMap_->erase(iter);
	return true;
}

void ShadersManager::Recycle(Shader* shader) 
{
	if (shader == NULL) return;
	if (shaderMap_ == NULL || shaderMap_->empty())
	{
		delete shader;
		return;
	}
	for (sshaderMap::iterator i = shaderMap_->begin(); i != shaderMap_->end(); i++) 
	{
		if (i->second != shader) continue;
		delete shader;
		shaderMap_->erase(i);
		return;
	}
}

Camera::Camera(glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch, float _speed, float _mouseSensitivity)
	:front(glm::vec3(0.0f, 0.0f, -1.0f)) , firstMouse_(true)
{
	speed = _speed;
	position = _position;
	worldUp = _up;
	yaw = _yaw;
	pitch = _pitch;
	mouseSensitivity = _mouseSensitivity;
	UpdateCameraVectors();
}

Camera::Camera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch, float _speed , float _mouseSensitivity )
	:front(glm::vec3(0.0f, 0.0f, -1.0f)) , firstMouse_(true)
{
	position = glm::vec3(_posX, _posY, _posZ);
	worldUp = glm::vec3(_upX, _upY, _upZ);
	yaw = _yaw;
	pitch = _pitch;
	UpdateCameraVectors();
}

void Camera::ProcessKeyboard(Camera::ECameraMovement direction, float delaTime)
{

	float velocity = speed * delaTime;
	if (direction == Camera::ECameraMovement::FORWARD)
	{
		position += front * velocity;
	}
	if (direction == Camera::ECameraMovement::BACKWARD)
	{
		position -= front * velocity;
	}
	if (direction == Camera::ECameraMovement::RIGHT)
	{
		position += right * velocity;
	}
	if (direction == Camera::ECameraMovement::LEFT)
	{
		position -= right * velocity;
	}
}

void Camera::UpdateCameraVectors() 
{
	glm::vec3 _front = glm::vec3(1.0f);

	float rYaw = glm::radians(yaw);
	float rPitch = glm::radians(pitch);

	_front.x = cos(rYaw) * cos(rPitch);
	_front.y = sin(rPitch);
	_front.z = sin(rYaw) * cos(rPitch);
	front = glm::normalize(_front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::GetViewMatrix() 
{
	return glm::lookAt(position, position + front, up);
}

void Camera::ProcessMouseMovement(float xpos, float ypos, bool constrainPitch) 
{
	if (firstMouse_) 
	{
		lastX_ = xpos;
		lastY_ = ypos;
		firstMouse_ = false;
	}

	float xoffset = xpos - lastX_;
	float yoffset = lastY_ - ypos;

	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch) 
	{
		pitch = Clamp(pitch, 89.9f, -89.9f);
	}
	UpdateCameraVectors();
}

