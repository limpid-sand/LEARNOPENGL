#ifndef __SHADER_H
#define __SHADER_H

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

#include<glad\glad.h>

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

private:
	enum class SHADER_TYPE {
		VERTEX = 0, FRAGEMENT
	};

	unsigned int create_shader(const char* shaderSource, SHADER_TYPE TYPE);

	unsigned int create_program(const char* vertexShaderSource, const char* fragmentShaderSource);

};

#endif
