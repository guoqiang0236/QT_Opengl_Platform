#version 450 core
in vec2 uv;
out vec4 FragColor;
uniform sampler2D logoTex;
void main() {
    FragColor = texture(logoTex, uv);
}
