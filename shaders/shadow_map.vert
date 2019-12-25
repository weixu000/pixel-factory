#version 450

layout (location = 0) in vec3 position;

uniform mat4 world;

void main() {
    gl_Position = world * vec4(position, 1.0);
}
