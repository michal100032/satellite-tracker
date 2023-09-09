#include "Mesh.hpp"

#include <iostream>

#include "consts.hpp"


Mesh::Mesh() {

}

Mesh::~Mesh()
{
}

void Mesh::init() {
	initializeBuffers();
}

void Mesh::generateSphere(int longitudanalResolution, int latitudinalResolution, float radius) {
	
	Vertex vertex = {
		glm::vec3(0.0f, radius, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 0.0f)
	};
	m_vertices.push_back(vertex);

	for (float y = 1.0f; y <= latitudinalResolution; y++) {
		float yAngle = PI / 2 * (1 - 2.0f * y / (latitudinalResolution + 1));

		vertex.textureCoords.y = y / (latitudinalResolution + 1);

		vertex.position.y = sinf(yAngle);
		float r = cosf(yAngle);

		for (float x = 0; x <= longitudanalResolution; x++) {
			float xAngle = PI * (2.0f * x / (float)longitudanalResolution - 1.0f);

			vertex.textureCoords.x = 1.0f - x / (float)longitudanalResolution;

			vertex.position.x = r * cosf(xAngle);
			vertex.position.z = r * sinf(xAngle);

			vertex.normal = vertex.position;
			vertex.position *= radius;

			m_vertices.push_back(vertex);
		}
	}
	vertex.position = glm::vec3(0.0f, -radius, 0.0f);
	vertex.normal = glm::vec3(0.0f, -1.0f, 0.0f);
	vertex.textureCoords = glm::vec2(0.5f, 1.0f);
	m_vertices.push_back(vertex);

	// north pole triangles
	for (int x = 1; x <= longitudanalResolution; x++) {
		m_indices.push_back(0);
		m_indices.push_back(x);
		m_indices.push_back(x + 1);
	}
	for (int y = 0; y < latitudinalResolution - 1; y++) {
		for (int x = 0; x <= longitudanalResolution - 1; x++) {
			int topLeftCorner = y * (longitudanalResolution + 1) + x + 1;

			m_indices.push_back(topLeftCorner);
			m_indices.push_back(topLeftCorner + 1);
			m_indices.push_back(topLeftCorner + longitudanalResolution + 1);

			m_indices.push_back(topLeftCorner + 1);
			m_indices.push_back(topLeftCorner + 1 + longitudanalResolution + 1);
			m_indices.push_back(topLeftCorner + longitudanalResolution + 1);

		}
	}

	// south pole triangles
	for (int x = m_vertices.size() - longitudanalResolution - 2; x < m_vertices.size() - 2; x++) {
		m_indices.push_back(m_vertices.size() - 1);
		m_indices.push_back(x);
		m_indices.push_back(x + 1);
	}
}

void Mesh::draw(const Shader& shader) {

	GLuint location = glGetUniformLocation(shader.id(), "tex0");
	glUniform1i(location, 0);
	
	glActiveTexture(GL_TEXTURE0);
	m_texture.bind();
	
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::loadTexture(const std::string& path) {
	m_texture.load(path);
}

void Mesh::initializeBuffers() {
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	// texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	// normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
