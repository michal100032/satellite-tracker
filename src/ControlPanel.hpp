#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <chrono>

class ControlPanel {
public:
	ControlPanel();
	~ControlPanel();

	void init(GLFWwindow* window);
	void update();
	void draw();

	void setCatalogNumber(int catalogNumber);
	void setTime(std::chrono::utc_clock::time_point date);

	bool focused();
	bool trackedObjectChanged();
	int catalogNumber();

	bool timeChanged();
	std::chrono::utc_clock::time_point time();

private:
	ImVec2 m_position;
	ImVec2 m_dimensions;

	ImFont* m_font;
	char m_catalogNumberBuffer[32];
	char m_timeBuffer[32];
	bool m_focused = false;
	bool m_trackedBtn = false;
	bool m_timeBtn = false;

	std::chrono::seconds m_offset;
	std::chrono::utc_clock::time_point m_time;
};