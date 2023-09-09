#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Texture {
private:
	int m_width = 0, m_height = 0, m_channelCount = 0;

	GLuint m_id = NULL;

public:
	Texture();
	Texture(const std::string& path);
	~Texture();

	void load(const std::string& path);
	void bind();

	GLuint id();
};

