#pragma once

#include <glm/glm.hpp>

constexpr double SCALE = 1.0 / 6371000.0;
constexpr double GRAV_PARAM = 3.986004e14f;

class Orbit {
public:
	Orbit();
	Orbit(double sma, double ecc, double raan, double incl, double aop, double ta);

	void update(double dt);
	bool isPrograde();
	glm::vec3 getPosition();
	
	static double meanFromEccentric(double ea, double ecc);
	static double eccentricFromTrue(double ta, double ecc);

	static double trueFromEccentric(double ea, double ecc);
	static double eccentricFromMean(double ma, double eccf);

	static double meanFromTrue(double ta, double ecc);
	static double trueFromMean(double ma, double ecc);
	
	glm::mat3 localToSpaceMatrix();

	double ecc() { return m_ecc;  }
	double sma() { return m_sma; }
	double incl() { return m_incl; }
	double aop() { return m_aop; }
	double raan() { return m_raan; }
	double slr() { return m_slr; }
	double ma() { return m_ma; }

private:
	void calculateMatrix();
	
	glm::mat3 m_matrix;

	double m_sma  = 0.0;  // semi-major axis
	double m_ecc  = 0.0;  // eccentricity
	double m_raan = 0.0;  // right ascension of the ascending node
	double m_incl = 0.0;  // inclination
	double m_aop  = 0.0;  // argument of perigee
	double m_ta   = 0.4;  // true anomaly

	double m_time = 0.0;  // time from perigee
	double m_ma   = 0.0;  // mean anomaly
	double m_slr  = 0.0;

	double m_meanAngular = 0.0;


};