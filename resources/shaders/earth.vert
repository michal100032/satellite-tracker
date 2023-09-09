#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;

out vec2 texCoord;
out vec3 normal;

uniform mat4 cameraMatrix;
uniform mat4 modelMatrix;

void main() {
    texCoord = tex;
    normal = mat3(transpose(inverse(modelMatrix))) * norm;
    gl_Position = cameraMatrix * modelMatrix * vec4(position, 1.0f);
}