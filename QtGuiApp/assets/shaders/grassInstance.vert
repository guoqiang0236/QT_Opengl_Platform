#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aColor;
layout (location = 4) in mat4 aInstanceMatrix;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;
out vec2 worldXZ;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
//uniform mat4 matrices[100];

uniform float time;

uniform float windScale;
uniform vec3 windDirection;
uniform float phaseScale;


uniform mat3 normalMatrix;

//aPos��Ϊattribute�����ԣ�����shader
//��������ĵ�
void main()
{
// ������Ķ���λ�ã�ת��Ϊ������꣨3ά-4ά��
	vec4 transformPosition = vec4(aPos, 1.0);

	//��һ���м����TransformPosition�����ڼ�����λλ����modelMatrix��˵��м���
	//transformPosition = modelMatrix * matrices[gl_InstanceID] * transformPosition;
	transformPosition = modelMatrix * aInstanceMatrix * transformPosition;
	worldXZ = transformPosition.xz;
	//�����䶯
	vec3 windDirN = normalize(windDirection);
	float phaseDistance = dot(windDirN, transformPosition.xyz);//��λ
	transformPosition += vec4(sin(time + phaseDistance / phaseScale) * (1.0 - aColor.r) * windScale * windDirN, 0.0);
	//���㵱ǰ�����worldPosition������������FragmentShader
	worldPosition = transformPosition.xyz;

	gl_Position = projectionMatrix * viewMatrix * transformPosition;
	
	uv = aUV;
	normal =  transpose(inverse(mat3(modelMatrix * aInstanceMatrix))) * aNormal;
	//normal =  normalMatrix * aNormal;
}