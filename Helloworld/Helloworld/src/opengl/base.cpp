#include<opengl/base.h>
#include<opengl/helperFun.h>

#ifdef _WIN32
#include <direct.h>
#elif __linux__ || __APPLE__
#include<unistd.h>
#endif

#ifdef _WIN32
#define GET_CWD(buffer,bytesSize) _getcwd(buffer, bytesSize)
#elif __linux__ || __APPLE__
#define GET_CWD(buffer,bytesSize) getcwd(buffer, bytesSize)
#else
#define GET_CWD(buffer,bytesSize) "ERROR::Can't get platform"
#endif // _WIN32


std::string System::GetCurrentWorkDir()const { return workDir_; }
std::string System::GetProjectDir()const { return projectDir_; }
System& System::Instance() 
{
	static System system;
	return system;
}

void System::Init(int args, char** argv)
{
	projectDir_ = argv[0];

	char* buffer = new char[256];
	buffer = GET_CWD(buffer, 256);
	workDir_ = buffer;
	delete[] buffer;
}

void System::Destroy() {}

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
		std::cout << "Create GL_FRAGMENT_SHADER Failed.Code:" << success << "Shader Source:" << fragment << std::endl << log << std::endl;
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

void Shader::SetMatrix3fv(const char* name, int matrixCount, GLboolean transpose, const float* value)
{
	int uniformID = glGetUniformLocation(programID_, name);
	//matrixCount:参数表示要发送给OpenGL多少个的举证
	//trnaspose: 表示是否对矩阵进行置换（行列交换）
	glUniformMatrix3fv(uniformID, matrixCount, transpose, value);
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

bool ShadersManager::NullOrEmpty() const
{
	if (shaderMap_ == NULL) return true;
	return shaderMap_->empty();
}

Shader* ShadersManager::GetShader(std::string shaderName) const
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

int ShadersManager::GetShaderCount() const
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

	float velocity = speed * delaTime;
	if (direction == FPS_Camera::ECameraMovement::FORWARD)
	{
		position += front * velocity;
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

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> indices, std::vector<Texture> textures) 
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupMesh();
}

void Mesh::Draw(Shader& shader) 
{
	uint diffuseNr = 1;
	uint specularNr = 1;

	for (uint i = 0, size = textures.size();i < size;i++) 
	{
		glActiveTexture(TextureNum(i));
		std::string num;
		std::string name = textures[i].type;
		if (name == TEXTURE_DIFFUSE)
		{
			num = std::to_string(specularNr++);
		}
		else if (name == TEXTURE_SPECULAR)
		{
			num = std::to_string(specularNr++);
		}
		else 
		{
			continue;
		}
		shader.SetFloat(("material." + name + num).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO_);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::SetupMesh() 
{
	glGenBuffers(1, &VBO_);
	glGenBuffers(1, &EBO_);
	glGenVertexArrays(1, &VAO_);

	glBindVertexArray(VAO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

}

Model::Model(const char* path) 
{
	LoadModel(path);
}

void Model::Draw(Shader& shader) 
{
	uint size = meshes_.size();
	for (int i = 0;i < size;i++) 
	{
		meshes_[i].Draw(shader);
	}
}

void Model::LoadModel(const char* path) 
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		std::cout << "ERROR:ASSIMP:" << importer.GetErrorString() << std::endl;
		return;
	}
	std::string str_path(path);
	directory_ = str_path.substr(0, str_path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (uint i = 0;i < node->mNumMeshes;i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes_.push_back(ProcessMesh(mesh, scene));
	}
	for (uint i = 0;i < node->mNumChildren;i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) 
{
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	std::vector<Texture> textures;

	//处理顶点
	for (uint i = 0;i < mesh->mNumVertices;i++) 
	{
		Vertex vertex;

		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		vertex.position = position;

		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		vertex.normal = normal;

		glm::vec2 texCoords;
		if (mesh->mTextureCoords[0]) 
		{
			texCoords.x = mesh->mTextureCoords[0][i].x;
			texCoords.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = texCoords;
		}
		else 
		{
			vertex.texCoords = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}

	//处理索引
	for (uint i = 0;i < mesh->mNumFaces;i++) 
	{
		aiFace face = mesh->mFaces[i];
		for (uint j = 0;j < face.mNumIndices;j++) 
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if(mesh->mMaterialIndex >= 0) 
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE);
		if (!diffuseMaps.empty()) 
		{
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		}
		else 
		{
			std::cout << "This mesh dosen't find texture_diffuse" << std::endl;
		}

		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TEXTURE_SPECULAR);

		if (!specularMaps.empty()) 
		{
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
		else 
		{
			std::cout << "This mesh doesn't find texture_specular" << std::endl;
		}
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	uint size = mat->GetTextureCount(type);
	for (uint i = 0;i < size;i++) 
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		bool loaded = false;
		uint loadedSize = textures_loaded.size();
		for (uint j = 0;j < loadedSize;j++) 
		{
			if (str.data == NULL || std::strlen(str.data) == 0) 
			{
				break;
			}
			if (std::strcmp(str.C_Str(), textures_loaded[j].path.C_Str()) )
			{
				textures.push_back(textures_loaded[j]);
				loaded = true;
				break;
			}
		}

		if (!loaded) 
		{

			bool success = LoadImage2D(&texture.id, str.C_Str(), GL_REPEAT, GL_NEAREST);
			if (!success)
			{
				texture.id = -1;
				texture.type = "none";
			}
			else
			{
				texture.type = typeName;
			}
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}

	}
	return textures;
}

void FPS_Camera::ProcessMouseScroll(float yoffset) 
{
	zoom -= yoffset;
	zoom = Clamp(zoom, 45.0f, 1.0f);
}

QuickString::QuickString() 
{
	capacity_ = 32;
	length_ = 0;
	str_ = new char[capacity_];
}

QuickString::QuickString(uint _capacity) 
{
	capacity_ = _capacity;
	length_ = 0;
	str_ = new char[capacity_];
}

QuickString::QuickString(const QuickString& _s) 
{
	capacity_ = _s.capacity_;
	length_ = _s.length_;
	str_ = new char[capacity_];
	//std::strcpy(str_, _s.str_);
}

QuickString::~QuickString() 
{
	delete str_;
	capacity_ = 0;
	length_ = 0;
}

void QuickString::ResetSize(uint _capacity) 
{
	if (length_ < _capacity) throw "The capacity is smaller than the current length";
	char* s = new char[_capacity];
	//std::strcpy(s, str_);
	delete[] str_;
	str_ = s;
}

QuickString& QuickString::Append(const char* _s) 
{
	uint size = std::strlen(_s) + length_;
	if (size >= capacity_) 
	{
		uint capacity = 2 * capacity_;
		do 
		{
			capacity *= 2;
		} while (capacity >size);
		ResetSize(capacity);
	}
	length_ = size;
	//std::strcat(str_, _s);
	return *this;
}

QuickString& QuickString::Append(const QuickString& _s) 
{
	uint size = std::strlen(_s.str_) + length_;
	if (size >= capacity_)
	{
		uint capacity = 2 * capacity_;
		do
		{
			capacity *= 2;
		} while (capacity > size);
		ResetSize(capacity);
	}
	length_ = size;
	//std::strcat(str_, _s.str_);
	return *this;
}

QuickString& QuickString::operator+(const char* _s) 
{
	return Append(_s);
}

QuickString& QuickString::operator+(const QuickString& _s)
{
	return Append(_s);
}

QuickString& QuickString::operator-(const char* _s)
{
	uint length = std::strlen(_s);
	if (length > length_) throw "string to long";

	uint index = length_ - length - 1;
	bool flag = true;
	for (int i = 0;i < length;i++)
	{
		if (str_[index++] != _s[i++]) 
		{
			flag = false;
			break;
		}
	}
	if (!flag) return *this;
	index = length_ - length - 1;
	for (;index < length_;index++) 
	{
		str_[index] = '\0';
	}
	return *this;
}

QuickString& QuickString::operator-(const QuickString& _s)
{
	uint length = _s.length_;
	if (length > length_) throw "string to long";

	uint index = length_ - length - 1;
	bool flag = true;
	for (int i = 0;i < length;i++)
	{
		if (str_[index++] != _s.str_[i++])
		{
			flag = false;
			break;
		}
	}
	if (!flag) return *this;
	index = length_ - length - 1;
	for (;index < length_;index++)
	{
		str_[index] = '\0';
	}
	return *this;
}

std::ostream& operator <<(std::ostream& _o, QuickString& _s)
{
	_o << _s.str_;
	return _o;
}