#pragma once

#include "imgui.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>

#include "Camera.hpp"
#include "Earth.hpp"

#include "Orbit/OrbitRenderer.hpp"
#include "Orbit/OrbitData.hpp"

#include "ControlPanel.hpp"


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

	Orbit m_orbit;
	OrbitRenderer m_orbRenderer;
	OrbitData m_data;

	ControlPanel m_panel;
};