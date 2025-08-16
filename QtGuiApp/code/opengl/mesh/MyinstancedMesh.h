#pragma once
#include "MyMesh.h"
#include <algorithm>

namespace MyOpenGL {
	class MyInstancedMesh :public MyMesh
	{
	public:
		MyInstancedMesh(MyGeometry* geometry, MyMaterial* material, unsigned int instanceCount);
		~MyInstancedMesh();

		
		

		unsigned int mInstanceCount{ 0 };
		std::vector<glm::mat4> mInstanceMatrices{};
		unsigned int mMatrixVbo{ 0 };
		void updateMatrices();
		void sortMatrices(glm::mat4 viewMatrix);
	};

}