#pragma once
#include "MyMaterial.h"
#include "../MyTexture.h"


namespace MyOpenGL {
	class MyGrassInstanceMaterial : public MyMaterial
	{
	public:
		MyGrassInstanceMaterial();
		~MyGrassInstanceMaterial();

		MyTexture* mDiffuse{ nullptr };
		MyTexture* mOpacityMask{ nullptr };
		float mShiness{ 1.0f };

		//�ݵ���ͼ����
		float mUVScale{ 1.0f };
		float mBrightness{ 1.0f };

		//�������
		float mWindScale{ 0.1f };
		glm::vec3 mWindDirection{ 1.0f, 1.0f, 1.0f };
		float mPhaseScale{ 1.0f };

		//�ƶ����
		MyTexture* mCloudMask{ nullptr };
		glm::vec3 mCloudWhiteColor{ 1.0,0.0,0.0 };
		glm::vec3 mCloudBlackColor{ 0.0,0.0,1.0 };
		float mCloudUVScale{ 1.0f };
		float mCloudSpeed{ 0.1f };
		float mCloudLerp{ 0.5f };

	private:

	};

}