#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>

#include <string>

#include "Mesh.hpp"
#include "Shader.hpp"

#include <chrono>

class Earth {
public:
	Earth();

	void loadTexture();
	void loadShader();

	void update(std::chrono::utc_clock::time_point time);
	void draw();

private:
	Shader m_shader;
	Mesh m_mesh;

	float m_rotation = 0.0f;
	float m_angularSpeed = 0.01f;

	GLuint m_modelMatrixUniform = NULL;
	GLuint m_cameraMatrixUniform = NULL;
	GLuint m_lightDirUniform = NULL;

	glm::mat4 m_model = glm::mat4(1.0f);
	glm::vec3 m_lightDir = glm::vec3(0.0f);

	friend class Application;
};