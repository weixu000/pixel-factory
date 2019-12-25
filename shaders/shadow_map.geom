#version 450
layout(triangles) in;
layout(triangle_strip, max_vertices = 3*6) out;

uniform mat4 lightProj;
uniform mat4 lightView[6];

out vec3 vPos;// position in light space

void main() {
    for (int face = 0; face < 6; face++) {
        for (int i = 0; i < 3; i++) {
            gl_Layer = face;
            vPos = vec3(lightView[face] * gl_in[i].gl_Position);
            gl_Position = lightProj * lightView[face] * gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();
    }
}
