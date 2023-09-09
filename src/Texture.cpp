#include "Texture.hpp"

#include <stb_image.h>

#include <iostream>

Texture::Texture() {

}

Texture::Texture(const std::string& path) {
	load(path);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_id);
}

void Texture::load(const std::string& path) {

	std::cout << path << std::endl;

	uint8_t* bytes = stbi_load(path.c_str(), &m_width, &m_height, &m_channelCount, 0);

	if (bytes == NULL) {
		std::cerr << "Texture error" << std::endl;
	}

	glGenTextures(1, &m_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, m_id);

}

GLuint Texture::id() {
	return m_id;
}