#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexFILE;
	std::ifstream fragmentFILE;
	vertexFILE.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFILE.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertexFILE.open(vertexPath);
		fragmentFILE.open(fragmentPath);

		std::stringstream ssVertex, ssFragment;
		ssVertex << vertexFILE.rdbuf();
		ssFragment << fragmentFILE.rdbuf();
		
		vertexCode = ssVertex.str();
		fragmentCode = ssFragment.str();

		vertexFILE.close();
		fragmentFILE.close();

	}
	catch(std::ifstream::failure e){
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ::" << e.what() <<std::endl;
	}
	
	const char* vShadeCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	this->ID = create_program(vShadeCode, fShaderCode);

}

unsigned int Shader::create_program(const char* vertexShaderSource, const char* fragmentShaderSource) {
	unsigned int program = glCreateProgram();
	unsigned int vertex_shader = create_shader(vertexShaderSource, SHADER_TYPE::VERTEX);
	unsigned int fragment_shader = create_shader(fragmentShaderSource, SHADER_TYPE::FRAGEMENT);

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cout << "ERROR::PROGRAM::SHADER::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return program;
}

unsigned int Shader::create_shader(const char* shaderSource, SHADER_TYPE TYPE) {
	unsigned int shader = 0;
	if(TYPE == SHADER_TYPE::VERTEX) shader = glCreateShader(GL_VERTEX_SHADER);
	else if(TYPE == SHADER_TYPE::FRAGEMENT) shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::" << ((TYPE == SHADER_TYPE::VERTEX) ?"VERTEX" : "FRAGEMENT") << "COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
}

void Shader::use()
{
	glUseProgram(this->ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	int location = glGetUniformLocation(this->ID,name.c_str());
	glUniform1i(location, (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	int location = glGetUniformLocation(this->ID, name.c_str());
	glUniform1i(location, value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	int location = glGetUniformLocation(this->ID, name.c_str());
	std::cout << location << std::endl;
	glUniform1f(location,value);
}
