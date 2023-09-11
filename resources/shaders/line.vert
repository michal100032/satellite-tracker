#version 330

layout(location = 0) in vec3 position;

uniform mat4 cameraMatrix;

void main() {
	gl_Position = cameraMatrix * vec4(position, 1.0f);
}