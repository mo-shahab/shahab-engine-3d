#version 330 core
out vec4 FragColor;

in vec3 Normal; // We need to pass this from the vertex shader

void main() {
    // This turns normals (-1.0 to 1.0) into colors (0.0 to 1.0)
    vec3 color = Normal * 0.5 + 0.5;
    FragColor = vec4(color, 1.0);
}