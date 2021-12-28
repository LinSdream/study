#include<opengl/shaderReader.h>
#include<opengl/helperFun.h>
#include<sstream>

ShaderReader::ShaderReader() 
{

}

ShaderReader::~ShaderReader()
{

}

std::string ShaderReader::Read(const char* path)
{
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try 
	{
		file.open(path);
		if (!file.good()) 
		{
			std::cout << "Open File Failed: " << path << std::endl;
			file.close();
			return EMPTY_STR;
		}

		std::stringstream stream;
		stream << file.rdbuf();
		file.close();
		return stream.str();
	}
	catch (std::ifstream::failure e) 
	{
		std::cout << "ERROR:SHADER::FILE_NOT_SUCCESFULLY_READ. IFSTREAM.FAILURE CODE: " << e.code() << std::endl;
		return EMPTY_STR;
	}
}