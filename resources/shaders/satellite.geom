#version 330

in vec3 vert_color[];
out vec3 geo_color;

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

void main() {
    vec4 position = gl_in[0].gl_Position / gl_in[0].gl_Position.w;

    float sideLength = 0.015;

    geo_color = vert_color[0];
	gl_Position = position + vec4(-sideLength, -sideLength, 0.0, 0.0);    // 1:bottom-left
    EmitVertex();   
    gl_Position = position + vec4(sideLength, -sideLength, 0.0, 0.0);    // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-sideLength,  sideLength, 0.0, 0.0);    // 3:top-left
    EmitVertex();
    gl_Position = position + vec4(sideLength, sideLength, 0.0, 0.0);    // 4:top-right
    EmitVertex();
    
    EndPrimitive();
}