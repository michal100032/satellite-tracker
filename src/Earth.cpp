#include "Earth.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "consts.hpp"
#include "Camera.hpp"
#include "Time.hpp"

#include <iostream>

const char* EARTH_TEXTURE_PATH = "resources/textures/earth_day.jpg";
const char* EARTH_VERTSH_PATH = "resources/shaders/earth.vert";
const char* EARTH_FRAGSH_PATH = "resources/shaders/earth.frag";

Earth::Earth() : m_mesh() {
	
}

void Earth::loadTexture() {
	m_mesh.generateSphere(16, 16, 1.0f);
	m_mesh.init();

	m_mesh.loadTexture(EARTH_TEXTURE_PATH);
}

void Earth::loadShader() {
	m_shader.load(EARTH_VERTSH_PATH, EARTH_FRAGSH_PATH);
	
	m_lightDirUniform = glGetUniformLocation(m_shader.id(), "lightDir");
	m_modelMatrixUniform = glGetUniformLocation(m_shader.id(), "modelMatrix");
	m_cameraMatrixUniform = glGetUniformLocation(m_shader.id(), "cameraMatrix");
}

void Earth::update(float deltaTime) {
	m_rotation = (Time::gmstNow() - 43082) / 86164 * 2 * PI;
	m_model = glm::rotate(glm::mat4(1.0f), m_rotation, UP);
}

void Earth::draw() {
	m_shader.use();

	float angle = -Time::julianSinceEquinox() / 365.25f * 2 * PI + PI;

	glUniform3f(m_lightDirUniform, cosf(angle), 0.0f, sinf(angle));

	glUniformMatrix4fv(m_modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(m_model));
	glUniformMatrix4fv(m_cameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(Camera::inst().matrix()));

	m_mesh.draw(m_shader);
}
