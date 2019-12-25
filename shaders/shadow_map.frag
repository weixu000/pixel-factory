#version 450

uniform float fallOff;

in vec3 vPos;// position in light space

void main(){
    gl_FragDepth = length(vPos) / fallOff;
}
