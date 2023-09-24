#include "Camera.hpp"

#include "consts.hpp"

#include <iostream>

Camera* Camera::m_inst = nullptr;

Camera::Camera() :
	m_projection(glm::perspective(
		CAM_FOV, (float)WINDOW_WIDTH / WINDOW_HEIGHT, CAM_NEAR_PLANE, CAM_FAR_PLANE
	)),
	m_cameraDistance(CAM_INIT_DIST),
	m_position(glm::vec3(CAM_INIT_DIST, 0.0f, 0.0f)),
	m_camMatrix() {
	m_inst = this;
}

Camera::~Camera() { }

Camera& Camera::inst() {
	return *m_inst;
}

void Camera::updateMatrix() {
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(m_position, glm::vec3(), UP);

	m_camMatrix = m_projection * view;
}

void Camera::update(GLFWwindow* window) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

		glm::dvec2 mouse;
		glfwGetCursorPos(window, &mouse.x, &mouse.y);

		if (!m_firstClick) {			
			m_polarPosition.x += (mouse.x - m_previousMousePosition.x) * CAM_DRAG_SPEED;
			float deltaY = mouse.y - m_previousMousePosition.y;
			float newY = deltaY * CAM_DRAG_SPEED + m_polarPosition.y;
			if (deltaY > 0.0f && newY > PI / 2 - 0.001f)
				m_polarPosition.y = PI / 2 - 0.001f;
			else if (deltaY < 0.0f && newY < -PI / 2 + 0.001f)
				m_polarPosition.y = -PI / 2 + 0.001f;
			else m_polarPosition.y = newY;

			m_position.x = m_cameraDistance * cosf(m_polarPosition.x) * cosf(m_polarPosition.y);
			m_position.z = m_cameraDistance * sinf(m_polarPosition.x) * cosf(m_polarPosition.y);
			m_position.y = m_cameraDistance * sinf(m_polarPosition.y);
		}
		else {
			m_firstClick = false;
		}
		m_previousMousePosition = mouse;
	}
	else if (!m_firstClick) {
		m_firstClick = true;
	}
}

void Camera::zoom(float howMuch) {
	float distanceDelta = howMuch * CAM_ZOOM_SPEED;
	if (m_cameraDistance - distanceDelta >= CAM_MIN_DIST)
		m_cameraDistance -= distanceDelta;
	else m_cameraDistance = CAM_MIN_DIST;

	m_position.x = m_cameraDistance * cosf(m_polarPosition.x) * cosf(m_polarPosition.y);
	m_position.z = m_cameraDistance * sinf(m_polarPosition.x) * cosf(m_polarPosition.y);
	m_position.y = m_cameraDistance * sinf(m_polarPosition.y);
}

void Camera::noFocus() {
	m_firstClick = true;
}

glm::mat4 Camera::matrix() {
	return m_camMatrix;
}