#include "ControlPanel.hpp"

#include <cstdlib>
#include <iostream>
#include <format>
#include <chrono>
#include <ctime>
#include <sstream>

#include "imgui_internal.h"

const float PANEL_WIDTH = 300;
const float PANEL_HEIGHT = 180;

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

ControlPanel::ControlPanel() : 
	m_position(WINDOW_WIDTH - PANEL_WIDTH - 50, WINDOW_HEIGHT - PANEL_HEIGHT - 50),
	m_dimensions(PANEL_WIDTH, PANEL_HEIGHT) {
	memset(m_catalogNumberBuffer, 0, sizeof(m_catalogNumberBuffer));
	memset(m_timeBuffer, 0, sizeof(m_timeBuffer));
}

ControlPanel::~ControlPanel() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ControlPanel::init(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	
	io.Fonts->AddFontFromFileTTF("resources\\fonts\\Roboto-Regular.ttf", 20.0f);
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding.x = style.WindowPadding.y = 15;
	style.Colors[ImGuiCol_Button] = { 0.25f, 0.2f, 0.5f, 1.0f };
	style.Colors[ImGuiCol_ButtonHovered] = { 0.3f, 0.3f, 0.3f, 1.0f };
	style.Colors[ImGuiCol_WindowBg] = { 0.1f, 0.1f, 0.1f, 1.0f };
	style.Colors[ImGuiCol_FrameBg] = { 0.3f, 0.3f, 0.3f, 1.0f };
}

void ControlPanel::update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ControlPanel::draw() {
	ImGui::SetNextWindowPos(m_position, ImGuiCond_Always);
	ImGui::SetNextWindowSize(m_dimensions, ImGuiCond_Always);

	ImGuiStyle& style = ImGui::GetStyle();

	ImGui::Begin("Control Panel", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

	ImGui::Text("Satellite NORAD code");

	m_focused = ImGui::IsWindowFocused();


	ImGui::PushItemWidth(PANEL_WIDTH - 2 * style.WindowPadding.x - 18 - 4 * style.FramePadding.x);
	ImGui::InputText("##NORAD", m_catalogNumberBuffer, 6, ImGuiInputTextFlags_NoHorizontalScroll);
	
	ImGui::SameLine();
	m_trackedBtn = ImGui::ArrowButton("NORADBtn", ImGuiDir_Right);

	ImGui::NewLine();


	ImGui::Text("Time (UTC)");
	if (ImGui::InputText("##date", m_timeBuffer, 32, ImGuiInputTextFlags_NoHorizontalScroll)) {
		using namespace std::chrono;

		std::tm tm = { };
		std::stringstream ss(m_timeBuffer);
		ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

		time_t sysTime = system_clock::to_time_t(sys_days{ year(tm.tm_year + 1900) / month(tm.tm_mon + 1) / day(tm.tm_mday) }
		+ hours{ tm.tm_hour } + minutes{ tm.tm_min } + seconds{ tm.tm_sec });

		m_time = std::chrono::utc_clock::from_sys(system_clock::from_time_t(sysTime));
	}
	if (!(ImGui::IsItemActive() && ImGui::TempInputIsActive(ImGui::GetActiveID()))) {
		std::string timeStr = std::format("{:%Y-%m-%d %X}", this->time());
		strcpy(m_timeBuffer, timeStr.c_str());
	}
	ImGui::PopItemWidth();
	ImGui::SameLine();
	m_timeBtn = ImGui::ArrowButton("DateBtn", ImGuiDir_Right);
	if (m_timeBtn) {
		setTime(m_time);
		std::cout << std::format("{:%Y-%m-%d %X}", m_offset + std::chrono::utc_clock::now()) << std::endl;
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ControlPanel::setCatalogNumber(int catalogNumber) {
	sprintf(m_catalogNumberBuffer, "%ld", catalogNumber);
}

void ControlPanel::setTime(std::chrono::utc_clock::time_point time) {
	m_offset = std::chrono::duration_cast<std::chrono::seconds>
		(time - std::chrono::utc_clock::now());
}

bool ControlPanel::focused() {
	return m_focused;
}

bool ControlPanel::trackedObjectChanged() {
	return m_trackedBtn;
}

int ControlPanel::catalogNumber() {
	int num = atoi(m_catalogNumberBuffer);
	std::cout << num << std::endl;
	return num;
}

bool ControlPanel::timeChanged() {
	return m_timeBtn;
}

std::chrono::utc_clock::time_point ControlPanel::time() {
	return std::chrono::utc_clock::now() + m_offset;
}
