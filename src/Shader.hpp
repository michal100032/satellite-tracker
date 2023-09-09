#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader {
private:
	GLuint m_shaderProgram = NULL;

	GLuint loadShader(GLuint shaderType, const std::string& path);
	void compileShader(GLuint shader);

	GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);
public:
	Shader();
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

	void load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void use() const;
	GLuint id() const;
};