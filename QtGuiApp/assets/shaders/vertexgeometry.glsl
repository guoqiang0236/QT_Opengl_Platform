#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix; //视图矩阵
uniform mat4 projectionMatrix; //投影矩阵



//aPos作为attribute（属性）传入shader
//不允许更改的
void main()
{
    //vec4 position =vec4(aPos, 1.0);

    // 将输入的顶点位置，转化为齐次坐标(3维->4维)
    vec4 transformPosition = vec4(aPos, 1.0);

    // 做一个中间变量transformPosition，用于计算四位位置与modelMatrix相乘的中间结果
    transformPosition = modelMatrix * transformPosition;

    // 计算当前顶点的worldPosition，并且向后传输给FragmentShader
    worldPosition = transformPosition.xyz;

    // 计算最终位置（包含投影和视图变换）
    vec4 position = projectionMatrix * viewMatrix * transformPosition;
    gl_Position = position;
 

    // 传递其他属性
    uv = aUV;
    normal= aNormal;
}
