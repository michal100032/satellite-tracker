#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	Camera();
	~Camera();

	static Camera& inst();

	void updateMatrix();
	void update(GLFWwindow* window);
	void zoom(float howMuch);
	void noFocus();

	glm::mat4 matrix();

private:
	static Camera* m_inst;

	float m_cameraDistance;

	glm::vec2 m_polarPosition = glm::vec2(0.0f, 0.0f);
	glm::dvec2 m_previousMousePosition = glm::dvec2(0.0f, 0.0f);

	bool m_firstClick = true;

	glm::mat4 m_camMatrix;
	glm::mat4 m_projection;
	
	glm::vec3 m_position;
};