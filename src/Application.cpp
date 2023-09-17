#include "Application.hpp"

#include "consts.hpp"

#include <iostream>

#include "Time.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Application::Application() {
	init();

	SatelliteInfo info = m_data.load()[18]; // NOAA 18

	m_orbit = info.orbit;
	printf("Name: %s\n", info.name.c_str());
	printf("SMA:  %f\n", m_orbit.sma());
	printf("Ecc:  %f\n", m_orbit.ecc());
	printf("Aop:  %f\n", m_orbit.aop() * 180 / PI);
	printf("Raan: %f\n", m_orbit.raan() * 180 / PI);
	printf("Incl: %f\n", m_orbit.incl() * 180 / PI);

	glfwSetWindowTitle(m_window, ("Satellite tracker | " + info.name).c_str());
	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double dx, double dy) {
		Camera::inst().zoom((float)dy);
	});

	m_earth.loadTexture();
	m_earth.loadShader();

	m_orbRenderer.loadShader();
	m_orbRenderer.setOrbit(&m_orbit);


	m_isRunning = true;
}

Application::~Application() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Application::handleEvents() {
	glfwPollEvents();
	if (glfwWindowShouldClose(m_window))
		m_isRunning = false;
}

void Application::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_earth.draw();

	glm::vec3 pos = m_orbit.getPosition();

	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), pos);
	glUniformMatrix4fv(glGetUniformLocation(m_earth.m_shader.id(), "modelMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));
	m_orbRenderer.draw();

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void Application::update() {
	m_camera.update(m_window);
	m_camera.updateMatrix();
	m_orbit.update(0.0166);
	m_earth.update(0.0166f);

	int err = glGetError();
	if (err != 0)
		std::cout << err << std::endl;
}

bool Application::running() {
	return m_isRunning;
}

void Application::init() {
	glfwInit();

	m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	glfwMakeContextCurrent(m_window);

	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST);
}