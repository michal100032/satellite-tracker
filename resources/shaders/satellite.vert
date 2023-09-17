#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 cameraMatrix;

out vec3 vert_color;

void main() {
	vert_color = color;
	gl_Position = cameraMatrix * vec4(position, 1.0f);
}