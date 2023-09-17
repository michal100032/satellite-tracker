#version 330

in vec3 geo_color;
out vec4 color;


void main() {
	color = vec4(geo_color, 1.0f);
}