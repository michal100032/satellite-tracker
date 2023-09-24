#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <chrono>

#include "Orbit.hpp"
#include "../Shader.hpp"

class OrbitRenderer {
public:
	OrbitRenderer();
	~OrbitRenderer();

	void loadShader();

	void update(std::chrono::utc_clock::time_point time);
	void draw();

	void setOrbit(Orbit* orbit);
private:
	void generateVertices(Orbit* orbit);
	void createBuffers();
	void updateBuffer();

	Orbit* m_orbit = nullptr;
	std::vector<glm::vec3> m_vertices;
	glm::vec3 m_satPositions[6];

	Shader m_orbShader;
	Shader m_satShader;

	GLuint m_orbVAO = NULL, m_orbVBO = NULL;
	GLuint m_satVAO = NULL, m_satVBO = NULL;

	GLuint m_orbCameraMatrixUniform = NULL;
	GLuint m_satCameraMatrixUniform = NULL;
};