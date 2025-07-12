#version 450 core
out vec4 FragColor;

uniform float time;

in vec3 color;
void main()
{
   //FragColor = vec4(color, 1.0f);
   float intensity = (sin(time) + 1.0) / 2.0;
   FragColor = vec4(vec3(intensity) + color, 1.0f);
}