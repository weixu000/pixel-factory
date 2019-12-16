#version 430

in vec3 vNormal, vPos;
in vec2 vTexCoord;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

void main() {
    // store the fragment position vector in the first gbuffer texture
    gPosition = vPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(vNormal);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(diffuseTex, vTexCoord).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(specularTex, vTexCoord).r;
}