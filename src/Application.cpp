#include "Application.hpp"

#include "consts.hpp"

#include <iostream>

Application::Application() {
	init();

	m_earth.loadTexture(EARTH_TEXTURE_PATH);
	m_earth.loadShader(EARTH_VERTSH_PATH, EARTH_FRAGSH_PATH);

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

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void Application::update() {
	m_camera.update(m_window);
	m_camera.updateMatrix();

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