#version 410

in vec4 position;
in vec2 texcoord;
out vec2 coord;

void main() {
    coord = abs(vec2(0., 1.0) - texcoord);
    gl_Position = vec4(position.xyz, 1.0);
}