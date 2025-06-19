#version 450 core
out vec4 FragColor; 

in vec3 color;
in vec2 uv;

uniform sampler2D sampler;

void main()
{
    // 直接使用传入的颜色
    FragColor = texture(sampler,uv);
}
