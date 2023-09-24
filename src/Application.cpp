#include "Application.hpp"

#include "consts.hpp"

#include <iostream>

#include "Time.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Application::Application() {
	init();
	m_panel.init(m_window);

	m_data.load();
	SatelliteInfo& info = m_data.get(33591);
	m_orbit = info.orbit;
	printf("Name: %s\n", info.name.c_str());
	printf("SMA:  %f\n", m_orbit.sma());
	printf("Ecc:  %f\n", m_orbit.ecc());
	printf("Aop:  %f\n", m_orbit.aop() * 180 / PI);
	printf("Raan: %f\n", m_orbit.raan() * 180 / PI);
	printf("Incl: %f\n", m_orbit.incl() * 180 / PI);

	glfwSetWindowTitle(m_window, ("Satellite tracker | " + info.name).c_str());

	m_panel.setCatalogNumber(info.catalogNumber);

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
	m_orbRenderer.draw();
	m_panel.draw();

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void Application::update() {
	if (m_panel.focused()) {
		m_camera.noFocus();
	} else  {
		m_camera.update(m_window);
		m_camera.updateMatrix();
	}
	if (m_panel.trackedObjectChanged()) {
		m_orbit = m_data.get(m_panel.catalogNumber()).orbit;
		m_orbRenderer.setOrbit(&m_orbit);
	}

	auto time = m_panel.time();

	m_orbRenderer.update(time);
	m_earth.update(time);
	m_panel.update();

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