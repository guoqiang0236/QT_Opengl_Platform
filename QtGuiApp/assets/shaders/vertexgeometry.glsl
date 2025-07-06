#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

out vec2 uv;
out vec3 normal;

uniform mat4 transform;
uniform mat4 viewMatrix; //视图矩阵
uniform mat4 projectionMatrix; //投影矩阵

//aPos作为attribute（属性）传入shader
//不允许更改的
void main()
{
    vec4 position =vec4(aPos, 1.0);
    position = projectionMatrix * viewMatrix * transform * position;
    gl_Position = position;
    uv = aUV;
    normal= aNormal;
}
