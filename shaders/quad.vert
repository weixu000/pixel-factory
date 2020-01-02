#version 430

out vec2 vTexCoord;

const vec2 quadVertices[4] = { vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0) };

void main() {
    gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
    vTexCoord = (quadVertices[gl_VertexID] + 1) / 2;
}
