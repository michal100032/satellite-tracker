#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Application {

public:
	Application();
	~Application();

	void handleEvents();
	void render();
	void update();

	bool running();
private:
	GLFWwindow* m_window = nullptr;

	bool m_isRunning = false;

	void init();
};