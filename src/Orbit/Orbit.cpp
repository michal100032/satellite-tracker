#include "Orbit.hpp"

#include <cmath>
#include <chrono>
#include <iostream>

#include "../consts.hpp"

Orbit::Orbit() { }

Orbit::Orbit(double sma, double ecc, double raan, double incl, double aop, double ta, std::chrono::utc_clock::time_point time) :
	m_sma(sma), m_ecc(ecc), m_raan(raan), m_incl(incl), m_aop(aop), 
	m_ta(ta), m_slr(sma * (1 - ecc * ecc)), m_time(time) {

	calculateMatrix();
	m_ma = meanFromTrue(m_ta, m_ecc);
	m_meanAngular = sqrt(GRAV_PARAM / (m_sma * m_sma * m_sma));
}

void Orbit::update(double dt) {

}

bool Orbit::isPrograde() {
	return m_incl > PI / 2;
}

glm::vec3 Orbit::getPositionAt(std::chrono::utc_clock::time_point time) {
	std::chrono::utc_clock::duration duration = time - m_time;
	double secs = std::chrono::duration<double>(duration).count();

	double ma = secs * m_meanAngular + m_ma;
	float ta = (float)trueFromMean(-ma, m_ecc);

	float distance = m_slr / (1 + m_ecc * cosf(ta));
	glm::vec3 position = {
		distance * cosf(ta),
		distance * sinf(ta),
		0.0f,
	};
	position = m_matrix * position;

	return { position.x, position.z, position.y };
}

glm::vec3 Orbit::getPosition() {
	float distance = m_slr / (1 + m_ecc * cosf(m_ta));
	glm::vec3 position = {
		distance * cosf(m_ta),
		distance * sinf(m_ta),
		0.0f,
	};
	position = m_matrix * position;
	glm::vec3 vertex = { position.x, position.z, position.y };

	return vertex;
}

glm::mat3 Orbit::localToSpaceMatrix() {
	return m_matrix;
}

double Orbit::meanFromEccentric(double ea, double ecc) {
	return ea - ecc * sin(ea);
}

double Orbit::eccentricFromTrue(double ta, double ecc) {
	return 2 * atan(sqrt((1 - ecc) / (1 + ecc)) * tan(ta / 2));
}

double Orbit::trueFromEccentric(double ea, double ecc) {
	return 2 * atan(sqrt((1 + ecc) / (1 - ecc)) * tan(ea / 2));
}

double Orbit::eccentricFromMean(double ma, double ecc) {

	const int iterations = 20;

	double appx = ma;
	for (int i = 0; i < iterations; i++) {
		appx = appx - (appx - ecc * sin(appx) - ma) / (1 - ecc * cos(appx));
	}
	return appx;
}

double Orbit::meanFromTrue(double ta, double ecc) {
	double eccentric = eccentricFromTrue(ta, ecc);
	return meanFromEccentric(eccentric, ecc);
}

double Orbit::trueFromMean(double ma, double ecc) {
	double eccentric = eccentricFromMean(ma, ecc);
	return trueFromEccentric(eccentric, ecc);
}

void Orbit::calculateMatrix() {
	float sinAop = sin(m_aop), cosAop = cos(m_aop);
	float sinIncl = sin(m_incl), cosIncl = cos(m_incl);
	float sinRaan = sin(m_raan + PI), cosRaan = cos(m_raan + PI);

	glm::mat3 aopMatrix(
		cosAop, -sinAop, 0.0f,
		sinAop, cosAop, 0.0f,
		0.0f, 0.0f, 1.0f
	);

	glm::mat3 inclMatrix(
		1.0f, 0.0f, 0.0f,
		0.0f, cosIncl, -sinIncl,
		0.0f, sinIncl, cosIncl
	);

	glm::mat3 raanMatrix(
		cosRaan, -sinRaan, 0.0f,
		sinRaan, cosRaan, 0.0f,
		0.0f, 0.0f, 1.0f
	);

	m_matrix = glm::mat3(SCALE)  * raanMatrix * inclMatrix * aopMatrix;
}
