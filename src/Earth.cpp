#include "Earth.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "consts.hpp"
#include "Camera.hpp"

#include <iostream>

Earth::Earth() : m_mesh() {
	
}

void Earth::loadTexture(const std::string& texturePath) {
	
	m_mesh.generateSphere(16, 16, 1.0f);
	m_mesh.init();

	m_mesh.loadTexture(texturePath);
}

void Earth::loadShader(const std::string& vertexPath, const std::string& fragmentPath) {
	m_shader.load(vertexPath, fragmentPath);
	m_shader.use();
	m_modelMatrixUniform = glGetUniformLocation(m_shader.id(), "modelMatrix");
	m_cameraMatrixUniform = glGetUniformLocation(m_shader.id(), "cameraMatrix");
}

void Earth::update(float deltaTime) {
	m_rotation += m_angularSpeed * deltaTime;
	m_model = glm::rotate(glm::mat4(1.0f), m_rotation, UP);
}

void Earth::draw() {
	m_shader.use();

	glUniformMatrix4fv(m_modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(m_model));
	glUniformMatrix4fv(m_cameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(Camera::inst().matrix()));

	m_mesh.draw(m_shader);
}
