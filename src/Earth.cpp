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

void Earth::update(std::chrono::utc_clock::time_point time) {
	m_rotation = (Time::gmstFromUtc(time) - 43082) / 86164 * 2 * PI;
	m_model = glm::rotate(glm::mat4(1.0f), m_rotation, UP);

	float sunPosAngle = Time::julianSinceEquinox(time) / 365.25f * 2 * PI;
	float sunLatitude = 23.45f / 180 * PI * sinf(sunPosAngle);
	m_lightDir = {
		cosf(sunPosAngle + PI) * cosf(sunLatitude),
		sinf(sunLatitude),
		-sinf(sunPosAngle + PI) * cosf(sunLatitude)
	};
}

void Earth::draw() {
	m_shader.use();

	glUniform3f(m_lightDirUniform, m_lightDir.x, m_lightDir.y, m_lightDir.z);
	glUniformMatrix4fv(m_modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(m_model));
	glUniformMatrix4fv(m_cameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(Camera::inst().matrix()));

	m_mesh.draw(m_shader);
}
