#version 410

in vec2 coord;
out vec4 out_color;

void main() {
    out_color = vec4(coord, 1.0, 1.0);
}