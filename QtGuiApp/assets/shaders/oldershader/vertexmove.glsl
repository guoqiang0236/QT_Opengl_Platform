#version 450 core
in vec3 aPos;
in vec3 aColor;

//全局时间
uniform float time;

out vec3 color;
void main()
{
   float dx = 0.3;
   float offset = sin(time) * dx; 
   gl_Position = vec4(aPos.x + offset, aPos.y + offset, aPos.z, 1.0);
   //color = aColor;
   color = aColor * (cos(time) + 1.0) / 2.0;
}