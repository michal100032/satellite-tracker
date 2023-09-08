#include "Application.hpp"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;
const char* WINDOW_TITLE = "Satellite tracker";

Application::Application() {
	init();
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
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void Application::update() {

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

	m_isRunning = true;
}
