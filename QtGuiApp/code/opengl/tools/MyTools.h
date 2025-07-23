#pragma once
#include "../../pch.h"

namespace MyOpenGL {
	class MyTools
	{
	public:
		MyTools();
		~MyTools();

		//����һ������,�⹹���е�λ��.��ת.������Ϣ
		static void decompose(glm::mat4 matrix, glm::vec3& position, glm::vec3& eulerAngle, glm::vec3& scale);
	};
}
