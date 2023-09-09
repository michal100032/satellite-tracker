#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Texture.hpp"
#include "Shader.hpp"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

class Mesh {
public:
	Mesh();
	~Mesh();
	
	void init();
	void generateSphere(int longitudanalResolution, int latitudinalResolution, float radius);
	
	void draw(const Shader& shader);
	void loadTexture(const std::string& path);
private:

	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;

	Texture m_texture;

	GLuint m_VBO = NULL, m_EBO = NULL, m_VAO = NULL;

	void initializeBuffers();

};