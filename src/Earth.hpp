#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>

#include <string>

#include "Mesh.hpp"
#include "Shader.hpp"

class Earth {
public:
	Earth();

	void loadTexture(const std::string& texturePath);
	void loadShader(const std::string& vertexPath, const std::string& fragmentPath);

	void update(float deltaTime);
	void draw();

private:
	Shader m_shader;
	Mesh m_mesh;

	float m_rotation = 0.0f;
	float m_angularSpeed = 0.01f;

	GLuint m_modelMatrixUniform = NULL;
	GLuint m_cameraMatrixUniform = NULL;

	glm::mat4 m_model = glm::mat4(1.0f);
};