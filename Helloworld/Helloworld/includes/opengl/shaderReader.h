#pragma once
#include<iostream>
#include<fstream>

class ShaderReader
{
public:
	ShaderReader();
	~ShaderReader();

	std::string Read(const char* path);
};

