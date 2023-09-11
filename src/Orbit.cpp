#include "Orbit.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "consts.hpp"

Orbit::Orbit(float sma, float ecc, float raan, float incl, float aop) :
	sma(sma), ecc(ecc), raan(raan), incl(incl), aop(aop) { }

OrbitRenderer::OrbitRenderer(int resolution) : 
	m_resolution(resolution) { }

OrbitRenderer::~OrbitRenderer() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void OrbitRenderer::loadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
	m_shader.load(vertexShaderPath, fragmentShaderPath);

	m_cameraMatrixUniform = glGetUniformLocation(m_shader.id(), "cameraMatrix");
}

void OrbitRenderer::draw() {
	m_shader.use();

	glUniformMatrix4fv(m_cameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(Camera::inst().matrix()));

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_LINE_LOOP, 0, m_vertices.size());
	glBindVertexArray(0);
}

void OrbitRenderer::setOrbit(Orbit* orbit) {
	generateVertices(orbit);
	
	if (m_VBO == NULL)
		createBuffers();
	else updateBuffer();
}

glm::mat3 OrbitRenderer::generateTransformMatrix(Orbit* orbit) {
	float sinAop = sinf(orbit->aop), cosAop = cosf(orbit->aop);
	float sinIncl = sinf(orbit->incl), cosIncl = cosf(orbit->incl);
	float sinRaan = sinf(orbit->raan), cosRaan = cosf(orbit->raan);

	glm::mat3 aopMatrix(
		cosAop, -sinAop,  0.0f,
		sinAop,  cosAop,  0.0f,
		  0.0f,    0.0f,  1.0f
	);

	glm::mat3 inclMatrix(
		1.0f,     0.0f,     0.0f,
		0.0f,  cosIncl, -sinIncl,
		0.0f,  sinIncl,  cosIncl
	);

	glm::mat3 raanMatrix(
		cosRaan, -sinRaan,  0.0f,
		sinRaan,  cosRaan,  0.0f,
	       0.0f,     0.0f,  1.0f
	);

	return raanMatrix * inclMatrix * aopMatrix;
}

void OrbitRenderer::generateVertices(Orbit* orb) {
	glm::mat3 transform = generateTransformMatrix(orb);

	float semiLatusRectum = orb->sma * (1 - orb->ecc * orb->ecc);

	for (int i = 0; i < m_resolution; i++) {
		float trueAnomaly = PI * (2 * (float) i / m_resolution - 1);
		float distance = semiLatusRectum / (1 + orb->ecc * cosf(trueAnomaly));
		glm::vec3 position = {
			distance * cosf(trueAnomaly),
			0.0f,
			distance * sinf(trueAnomaly),
		};

		m_vertices.push_back(transform * position);
	}
}

void OrbitRenderer::createBuffers(){

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_resolution, &m_vertices[0], GL_DYNAMIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void OrbitRenderer::updateBuffer() {
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * m_resolution, &m_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}
