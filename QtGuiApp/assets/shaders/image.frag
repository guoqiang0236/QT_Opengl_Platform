#version 450 core
in vec3 color;
in vec2 uv;

out vec4 FragColor;

uniform sampler2D imagesampler;

void main()
{
  FragColor = texture(imagesampler, uv); 
}