#include "MyGeometry.h"

MyGeometry::MyGeometry(QObject* parent)
{
    initializeOpenGLFunctions();
}

MyGeometry::~MyGeometry()
{
	if (mVao != 0)
	{
		glDeleteVertexArrays(1, &mVao);
		mVao = 0;
	}
	if (mPosVbo != 0)
	{
		glDeleteBuffers(1, &mPosVbo);
		mPosVbo = 0;
	}
	if (mUvVao != 0)
	{
		glDeleteBuffers(1, &mUvVao);
		mUvVao = 0;
	}
	if (mEbo != 0)
	{
		glDeleteBuffers(1, &mEbo);
		mEbo = 0;
	}
}

MyGeometry* MyGeometry::createBox(float size)
{
	MyGeometry* geometry = new MyGeometry();
    // 立方体顶点数据（边长 = 2 * halfSize）
    const float halfSize = 0.5f; // 立方体半边长

	geometry->mIndicesCount = 36; // 立方体有6个面，每个面2个三角形，每个三角形3个顶点，总共6*2*3=36个顶点

    // 顶点位置数组 (24个顶点，每个面4个顶点)
    float positions[] = {
        // 前面 (Front)
        -halfSize, -halfSize,  halfSize, // 左下
         halfSize, -halfSize,  halfSize, // 右下
         halfSize,  halfSize,  halfSize, // 右上
        -halfSize,  halfSize,  halfSize, // 左上

        // 后面 (Back)
         halfSize, -halfSize, -halfSize, // 右下
        -halfSize, -halfSize, -halfSize, // 左下
        -halfSize,  halfSize, -halfSize, // 左上
         halfSize,  halfSize, -halfSize, // 右上

         // 左面 (Left)
         -halfSize, -halfSize, -halfSize, // 左下
         -halfSize, -halfSize,  halfSize, // 右下
         -halfSize,  halfSize,  halfSize, // 右上
         -halfSize,  halfSize, -halfSize, // 左上

         // 右面 (Right)
          halfSize, -halfSize,  halfSize, // 左下
          halfSize, -halfSize, -halfSize, // 右下
          halfSize,  halfSize, -halfSize, // 右上
          halfSize,  halfSize,  halfSize, // 左上

          // 上面 (Top)
          -halfSize,  halfSize,  halfSize, // 左下
           halfSize,  halfSize,  halfSize, // 右下
           halfSize,  halfSize, -halfSize, // 右上
          -halfSize,  halfSize, -halfSize, // 左上

          // 下面 (Bottom)
          -halfSize, -halfSize, -halfSize, // 左下
           halfSize, -halfSize, -halfSize, // 右下
           halfSize, -halfSize,  halfSize, // 右上
          -halfSize, -halfSize,  halfSize  // 左上
    };

    // UV坐标数组 (每个面4个UV坐标)
    float uvs[] = {
        // 每个面的UV坐标顺序: 左下(0,0), 右下(1,0), 右上(1,1), 左上(0,1)
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f, // Front
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f, // Back
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f, // Left
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f, // Right
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f, // Top
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f  // Bottom
    };

    // 索引数组 (36个索引，每个面6个索引)
    unsigned int indices[] = {
        // 前面
        0, 1, 2,  2, 3, 0,
        // 后面
        4, 5, 6,  6, 7, 4,
        // 左面
        8, 9, 10,  10, 11, 8,
        // 右面
        12, 13, 14,  14, 15, 12,
        // 上面
        16, 17, 18,  18, 19, 16,
        // 下面
        20, 21, 22,  22, 23, 20
    };
	
    // VBO创建
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVao, ebo = geometry->mEbo;
    //POS
    geometry->glGenBuffers(1, &posVbo);
	geometry->glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	geometry->glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    //UV
	geometry->glGenBuffers(1, &uvVbo);
	geometry->glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	geometry->glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//EBO
	geometry->glGenBuffers(1, &ebo);
	geometry->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	geometry->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
    // VAO创建
	geometry->glGenVertexArrays(1, &geometry->mVao);
	geometry->glBindVertexArray(geometry->mVao);

	// 绑定顶点位置
	geometry->glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    geometry->glEnableVertexAttribArray(0);
    geometry->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 绑定UV坐标
	geometry->glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	geometry->glEnableVertexAttribArray(1);
	geometry->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 绑定EBO
	geometry->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    geometry->glBindVertexArray(0);
	return geometry;
}

MyGeometry* MyGeometry::createSphere(float size)
{
	MyGeometry* geometry = new MyGeometry();

	return geometry;
}
