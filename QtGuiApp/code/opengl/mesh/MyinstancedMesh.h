#pragma once
#include "MyMesh.h"
#include <algorithm>

namespace MyOpenGL {
	class MyInstancedMesh :public MyMesh
	{
	public:
		MyInstancedMesh(MyGeometry* geometry, MyMaterial* material, unsigned int instanceCount);
		~MyInstancedMesh();


		void updateMatrices();

		unsigned int mInstanceCount{ 0 };
		glm::mat4*  mInstanceMatrices{nullptr};
		unsigned int mMatrixVbo{ 0 };
	};

}