#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "Earth.hpp"

class Application {

public:
	Application();
	~Application();

	void handleEvents();
	void render();
	void update();

	bool running();
private:
	void init();
	
	GLFWwindow* m_window = nullptr;

	bool m_isRunning = false;

	Camera m_camera;
	Earth m_earth;
};