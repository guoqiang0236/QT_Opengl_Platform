#version 450 core
in vec3 aPos;
in vec3 aColor;

//全局时间
uniform float time;

out vec3 color;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   //color = aColor;
   color = aColor * (cos(time) + 1.0) / 2.0;
}