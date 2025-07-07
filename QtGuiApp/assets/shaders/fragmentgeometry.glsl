#version 450 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;
//光源参数
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float specularIntensity; //镜面反射强度
uniform vec3 ambientColor; //环境光颜色

//世界摄像机位置
uniform vec3 cameraPosition;


void main()
{
  // 1 计算光照的通用数据
  vec3 objectColor = texture(sampler, uv).xyz;
  vec3 normalizedNormal = normalize(normal); // 将插值后的法线归一化，确保其长度为1，避免光照计算误差
  vec3 lightDirN = normalize(lightDirection); // 将光源方向归一化，确保其长度为1
  vec3 viewDir = normalize(worldPosition - cameraPosition); // 视线方向

  // 2 计算diffuse(漫反射)相关数据
  float diffuse = clamp(dot(-lightDirN, normalizedNormal),0.0,1.0);  // 计算漫反射分量： dot(-lightDirN, normalizedNormal) 表示光线入射方向与表面法线的夹角余弦
  vec3 diffuseColor = lightColor * diffuse * objectColor; // 计算最终颜色：光源颜色 * 漫反射系数 * 物体本身颜色

  // 3 计算specular (镜面反射)相关数据
  float doResult = dot(-lightDirN, normalizedNormal);// 防止背面光效果
  float flag = step(0.0, doResult); // 如果doResult小于0，则flag为0，否则为1
  vec3 lightReflect = normalize(reflect(lightDirN, normalizedNormal)); // 计算光线反射方向
  float specular = clamp(dot(lightReflect, -viewDir), 0.0, 1.0); // 计算镜面反射分量
  specular = pow(specular, 32.0); // 控制光斑大小，32.0是一个常用的值，可以根据需要调整
  vec3 specularColor = lightColor * specular * flag * specularIntensity;

  // 4 计算环境光
  vec3 ambientColorFinal = ambientColor * objectColor; // 环境光颜色与物体颜色的乘积


  vec3 finalColor = diffuseColor + specularColor + ambientColorFinal; // 最终颜色为漫反射和镜面反射和环境光的和

  FragColor = vec4(finalColor, 1.0); // 将视线方向与颜色结合

  
}