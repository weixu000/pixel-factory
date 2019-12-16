#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

// In world coordinates
out vec3 vNormal, vPos;
out vec2 vTexCoord;

void main() {
    gl_Position = projection * view * world * vec4(position, 1.0);

    vPos = vec3(world * vec4(position, 1.0));
    vNormal = transpose(inverse(mat3(world))) * normal;
    vTexCoord = texCoord;
}
