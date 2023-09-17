#version 330

in vec2 texCoord;
in vec3 normal;

out vec4 outputColor;

uniform sampler2D tex0;
uniform vec3 lightDir;

void main() {
	float intensity = max(dot(normal, lightDir), 0.0f) * 0.85f + 0.15f; 
	outputColor = texture(tex0, texCoord) * intensity;
}