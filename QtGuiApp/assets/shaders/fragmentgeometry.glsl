#version 450 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;

uniform sampler2D sampler;
//光源参数
uniform vec3 lightDirection;
uniform vec3 lightColor;

void main()
{
  // 1 获取物体当前像素的颜色
  vec3 objectColor = texture(sampler, uv).xyz;

  // 2 准备diffuse(漫反射)相关数据

  // 将插值后的法线归一化，确保其长度为1，避免光照计算误差
  vec3 normalizedNormal = normalize(normal);

  // 将光源方向归一化，确保其长度为1
  vec3 lightDirN = normalize(lightDirection);
  
  // 计算漫反射分量：
  // dot(-lightDirN, normalizedNormal) 表示光线入射方向与表面法线的夹角余弦
  // 取负号是因为lightDirection通常为“光线从光源指向物体”的方向，
  // 而漫反射需要“从物体指向光源”的方向
  float diffuse = clamp(dot(-lightDirN, normalizedNormal),0.0,1.0);
  
  // 计算最终颜色：光源颜色 * 漫反射系数 * 物体本身颜色
  vec3 finalColor = lightColor * diffuse * objectColor;

  FragColor =vec4(finalColor, 1.0);


  
}