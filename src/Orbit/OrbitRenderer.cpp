#include "OrbitRenderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Camera.hpp"
#include "../consts.hpp"

#include <iostream>
#include <chrono>

const int ORB_RESOLUTION = 50;
const char* ORB_VERTSH_PATH = "resources/shaders/line.vert";
const char* ORB_FRAGSH_PATH = "resources/shaders/line.frag";

const char* SAT_VERTSH_PATH = "resources/shaders/satellite.vert";
const char* SAT_GEOMSH_PATH = "resources/shaders/satellite.geom";
const char* SAT_FRAGSH_PATH = "resources/shaders/satellite.frag";

OrbitRenderer::OrbitRenderer() {
	m_satPositions[1] = { 1.0f, 1.0f, 1.0f };
	m_satPositions[3] = { 1.0f, 0.0f, 0.0f };
	m_satPositions[5] = { 0.0f, 0.0f, 1.0f };
}

OrbitRenderer::~OrbitRenderer() {
	glDeleteVertexArrays(1, &m_satVAO);
	glDeleteBuffers(1, &m_satVBO);
}

void OrbitRenderer::loadShader() {
	m_orbShader.load(ORB_VERTSH_PATH, ORB_FRAGSH_PATH);
	m_satShader.load(SAT_VERTSH_PATH, SAT_GEOMSH_PATH, SAT_FRAGSH_PATH);
	m_orbCameraMatrixUniform = glGetUniformLocation(m_orbShader.id(), "cameraMatrix");
	m_satCameraMatrixUniform = glGetUniformLocation(m_satShader.id(), "cameraMatrix");
}

void OrbitRenderer::update(std::chrono::utc_clock::time_point time) {
	m_satPositions[0] = m_orbit->getPositionAt(time);

	glBindBuffer(GL_ARRAY_BUFFER, m_satVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_satPositions), m_satPositions);
	glBindBuffer(GL_ARRAY_BUFFER, m_satVBO);

}

void OrbitRenderer::draw() {
	if (!m_orbit)
		return;

	m_orbShader.use();
	glUniformMatrix4fv(m_orbCameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(Camera::inst().matrix()));

	glBindVertexArray(m_orbVAO);
	glDrawArrays(GL_LINE_LOOP, 0, m_vertices.size());
	glBindVertexArray(0);

	m_satShader.use();
	glUniformMatrix4fv(m_satCameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(Camera::inst().matrix()));

	glBindVertexArray(m_satVAO);
	glDrawArrays(GL_POINTS, 0, 3);
	glBindVertexArray(0);
}

void OrbitRenderer::setOrbit(Orbit* orbit) {
	generateVertices(orbit);
	
	if (m_orbVBO == NULL)
		createBuffers();
	else updateBuffer();

	m_orbit = orbit;
}

void OrbitRenderer::generateVertices(Orbit* orb) {
	glm::mat3 transform = orb->localToSpaceMatrix();
	m_vertices.clear();

	for (int i = 0; i < ORB_RESOLUTION; i++) {
		float trueAnomaly = PI * (2 * (float) i / ORB_RESOLUTION - 1);
		float distance = orb->slr() / (1 + orb->ecc() * cosf(trueAnomaly));
		glm::vec3 position = {
			distance * cosf(trueAnomaly),
			distance * sinf(trueAnomaly),
			0.0f,
		};
		position = transform * position;
		glm::vec3 vertex = { position.x, position.z, position.y };
		m_vertices.push_back(vertex);
	}

	// periapsis
	glm::vec3 periPosition = { orb->slr() / (1 + orb->ecc()), 0.0f, 0.0f };
	periPosition = transform * periPosition;
	m_satPositions[2] = { periPosition.x, periPosition.z, periPosition.y };

	// apoapsis
	glm::vec3 apoPosition = { -orb->slr() / (1 - orb->ecc()), 0.0f, 0.0f };
	apoPosition = transform * apoPosition;
	m_satPositions[4] = { apoPosition.x, apoPosition.z, apoPosition.y };
}

void OrbitRenderer::createBuffers(){

	glGenVertexArrays(1, &m_orbVAO);
	glBindVertexArray(m_orbVAO);

	glGenBuffers(1, &m_orbVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_orbVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ORB_RESOLUTION, &m_vertices[0], GL_DYNAMIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenVertexArrays(1, &m_satVAO);
	glBindVertexArray(m_satVAO);

	glGenBuffers(1, &m_satVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_satVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(m_satPositions), m_satPositions, GL_STREAM_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	// color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OrbitRenderer::updateBuffer() {
	std::cout << "v " << m_vertices.size() << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, m_orbVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * m_vertices.size(), &m_vertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}