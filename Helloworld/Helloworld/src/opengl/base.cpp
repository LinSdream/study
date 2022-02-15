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

void Shader::Set4fv(const char* name, const float* value)
{
	int uniformID = glGetUniformLocation(programID_, name);
	glUniform4fv(uniformID, 1, value);
}

void Shader::Set3f(const char* name, float x, float y, float z)
{
	int uniformID = glGetUniformLocation(programID_, name);
	glUniform3f(uniformID, x, y, z);
}

void Shader::Set3fv(const char* name, const float* value) 
{
	int uniformID = glGetUniformLocation(programID_, name);
	glUniform3fv(uniformID, 1, value);
}

void Shader::Set2f(const char* name, float x, float y)
{
	int uniformID = glGetUniformLocation(programID_, name);
	glUniform2f(uniformID, x, y);
}

void Shader::Set2fv(const char* name, const float* value)
{
	int uniformID = glGetUniformLocation(programID_, name);
	glUniform2fv(uniformID, 1, value);
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

FPS_Camera::FPS_Camera(glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch, float _speed, float _zoom, float _mouseSensitivity)
	:front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(_speed), mouseSensitivity(_mouseSensitivity), zoom(_zoom)
{
	position = _position;
	worldUp = _up;
	yaw = _yaw;
	pitch = _pitch;
	UpdateCameraVectors();
}

FPS_Camera::FPS_Camera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch, float _speed, float _mouseSensitivity)
	:front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(_speed), mouseSensitivity(_mouseSensitivity)
{
	position = glm::vec3(_posX, _posY, _posZ);
	worldUp = glm::vec3(_upX, _upY, _upZ);
	yaw = _yaw;
	pitch = _pitch;
	UpdateCameraVectors();
}

void FPS_Camera::ProcessKeyboard(FPS_Camera::ECameraMovement direction, float delaTime)
{

	//front = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
	//right = glm::normalize(glm::cross(front, worldUp));

	//std::cout << "move before(" << position.x << "," << position.y << "," << position.z << ")" << std::endl;

	float velocity = speed * delaTime;
	if (direction == FPS_Camera::ECameraMovement::FORWARD)
	{
		position += front * velocity;
		//std::cout << "after(" << position.x << "," << position.y << "," << position.z << ")" << std::endl;
	}
	if (direction == FPS_Camera::ECameraMovement::BACKWARD)
	{
		position -= front * velocity;
	}
	if (direction == FPS_Camera::ECameraMovement::RIGHT)
	{
		position += right * velocity;
	}
	if (direction == FPS_Camera::ECameraMovement::LEFT)
	{
		position -= right * velocity;
	}
}

void FPS_Camera::UpdateCameraVectors() 
{

	//glm::mat4 rotateMat = glm::mat4(1.0f);
	//rotateMat = glm::rotate(rotateMat, glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	//rotateMat = glm::rotate(rotateMat, glm::radians(pitch), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::vec4 f = rotateMat * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
	//front = glm::normalize(glm::vec3(f.x, f.y, f.z));
	//right = glm::normalize(glm::cross(front, worldUp));
	//up = glm::normalize(glm::cross(front, right));

	glm::vec3 _front = glm::vec3(1.0f);

	float rYaw = glm::radians(yaw);
	float rPitch = glm::radians(pitch);

	_front.x = cos(rYaw) * cos(rPitch);
	_front.y = sin(rPitch);
	_front.z = sin(rYaw) * cos(rPitch);
	front = glm::normalize(_front);
	//std::cout << "Front: (" << front.x << "," << front.y << "," << front.z << ")" << std::endl;
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

}

glm::mat4 FPS_Camera::GetViewMatrix() 
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 FPS_Camera::GetProjectionMatrix(float aspectRatio, float zNear, float zFar)
{
	return glm::perspective(glm::radians(zoom), aspectRatio, zNear, zFar);
}

glm::mat4 FPS_Camera::CalculateLookAtMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) 
{
	glm::vec3 zaxis = glm::normalize(position - target);
	glm::vec3 xaxis = glm::normalize(glm::cross(worldUp, zaxis));
	glm::vec3 yaxis = glm::cross(zaxis, xaxis);
	glm::mat4 translation = glm::mat4(1.0f);
	translation[3][0] = -position.x;
	translation[3][1] = -position.y;
	translation[3][2] = -position.z;
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = xaxis.x; // First column, first row
	rotation[1][0] = xaxis.y;
	rotation[2][0] = xaxis.z;
	rotation[0][1] = yaxis.x; // First column, second row
	rotation[1][1] = yaxis.y;
	rotation[2][1] = yaxis.z;
	rotation[0][2] = zaxis.x; // First column, third row
	rotation[1][2] = zaxis.y;
	rotation[2][2] = zaxis.z;
	return rotation * translation;
}

void FPS_Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
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

void FPS_Camera::ProcessMouseScroll(float yoffset) 
{
	zoom -= yoffset;
	zoom = Clamp(zoom, 45.0f, 1.0f);
}