#include "Shader.hpp"

#include <iostream>

#include <fstream>
#include <sstream>

std::string readFile(const std::string& path) {
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}


Shader::Shader() {

}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderPath);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

	m_shaderProgram = createProgram(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader() {
	glDeleteProgram(m_shaderProgram);
}

GLuint Shader::loadShader(GLuint shaderType, const std::string& path) {
	std::string shaderSource = readFile(path);
	const char* shaderS = shaderSource.c_str();

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderS, 0);
	compileShader(shader);

	return shader;
}

void Shader::compileShader(GLuint shader) {
	char infoLog[512];
	int success;

	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cout << "Shader compilation failed" << std::endl << infoLog << std::endl;
	}
}

GLuint Shader::createProgram(GLuint vertexShader, GLuint fragmentShader) {
	char infoLog[512];
	int success;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "Shader program linking failed" << std::endl << infoLog << std::endl;
	}
	return program;
}

void Shader::load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderPath);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

	m_shaderProgram = createProgram(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() const {
	glUseProgram(m_shaderProgram);
}

GLuint Shader::id() const {
	return m_shaderProgram;
}
