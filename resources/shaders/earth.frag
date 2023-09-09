#version 330

in vec2 texCoord;
in vec3 normal;

out vec4 outputColor;

uniform sampler2D tex0;

vec3 light = vec3(1.0f, 0.0f, 0.0f); 


void main() {
	float intensity = max(dot(normal, light), 0.0f) * 0.85f + 0.15f; 
	outputColor = texture(tex0, texCoord) * intensity;
}