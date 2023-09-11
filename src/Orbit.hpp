#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "Shader.hpp"

constexpr float GRAV_PARAM = 3.986004e14f;

struct Orbit {
	Orbit(float sma, float ecc, float raan, float incl, float aop);

	float sma;    // semi-major axis
	float ecc;    // eccentricity
	float raan;   // right ascension of the ascending node
	float incl;   // inclination
	float aop;    // argument of perigee
};

class OrbitRenderer {
public:
	OrbitRenderer(int resolution);
	~OrbitRenderer();

	void loadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void draw();

	void setOrbit(Orbit* orbit);
private:
	glm::mat3 generateTransformMatrix(Orbit* orbit);
	void generateVertices(Orbit* orbit);
	void createBuffers();
	void updateBuffer();

	Orbit* m_orbit = nullptr;
	std::vector<glm::vec3> m_vertices;

	Shader m_shader;

	GLuint m_VAO = NULL, m_VBO = NULL;
	GLuint m_cameraMatrixUniform = NULL;

	int m_resolution;
};