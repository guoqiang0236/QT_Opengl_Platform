#pragma once
#include "../../pch.h"

namespace MyOpenGL {
	class MyTools
	{
	public:
		MyTools();
		~MyTools();

		//传入一个矩阵,解构其中的位置.旋转.缩放信息
		static void decompose(glm::mat4 matrix, glm::vec3& position, glm::vec3& eulerAngle, glm::vec3& scale);
	};
}
