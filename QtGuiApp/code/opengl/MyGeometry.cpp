#include "MyGeometry.h"
namespace MyOpenGL {
    MyGeometry::MyGeometry(QObject* parent)
    {
        initializeOpenGLFunctions();
    }

    MyGeometry::MyGeometry(
        const std::vector<float>& positions,
        const std::vector<float>& normals,
        const std::vector<float>& uvs,
        const std::vector<unsigned int>& indices, QObject* parent)
    {
        initializeOpenGLFunctions();
        mIndicesCount = indices.size();

        // VBO创建
        //POS
        glGenBuffers(1, &mPosVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

        //UV
        glGenBuffers(1, &mUvVao);
        glBindBuffer(GL_ARRAY_BUFFER, mUvVao);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

        // 法线
        glGenBuffers(1, &mNormalVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

        //EBO
        glGenBuffers(1, &mEbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // VAO创建
        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);

        // 绑定顶点位置
        glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // 绑定UV坐标
        glBindBuffer(GL_ARRAY_BUFFER, mUvVao);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

        // 绑定法线
        glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // 绑定EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);

        glBindVertexArray(0);

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
        if (mNormalVbo != 0)
        {
            glDeleteBuffers(1, &mNormalVbo);
            mNormalVbo = 0;
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

        //法线
        float normals[] = {
            // 前面 (Front)
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            // 后面 (Back)
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,

            // 左面 (Left)
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,

            // 右面 (Right)
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            // 上面 (Top)
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // 下面 (Bottom)
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f
        };


        // VBO创建
        GLuint& posVbo = geometry->mPosVbo,
            uvVbo = geometry->mUvVao,
            ebo = geometry->mEbo,
            normalVbo = geometry->mNormalVbo;

        //POS
        geometry->glGenBuffers(1, &posVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

        //UV
        geometry->glGenBuffers(1, &uvVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

        // 法线
        geometry->glGenBuffers(1, &normalVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

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
        geometry->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // 绑定UV坐标
        geometry->glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        geometry->glEnableVertexAttribArray(1);
        geometry->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

        // 绑定法线
        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glEnableVertexAttribArray(2);
        geometry->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // 绑定EBO
        geometry->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        geometry->glBindVertexArray(0);
        return geometry;
    }

    MyGeometry* MyGeometry::createSphere(float size)
    {
        MyGeometry* geometry = new MyGeometry();

        // 声明纬线与经线的数量
        const int numLatLines = 60;  // 纬线
        const int numLongLines = 60; // 经线
        const float radius = size * 0.5f;

        std::vector<float> positions;
        std::vector<float> uvs;
        std::vector<float> normals;
        std::vector<unsigned int> indices;

        // 通过两层循环（纬线在外，经线在内）->位置、uv
        for (int i = 0; i <= numLatLines; i++) {
            float phi = i * glm::pi<float>() / numLatLines;  // 纬度 [0, pi]
            float xy = radius * sinf(phi);
            float z = radius * cosf(phi);

            for (int j = 0; j <= numLongLines; j++) {
                float theta = j * 2 * glm::pi<float>() / numLongLines;  // 经度 [0, 2pi]
                float x = xy * cosf(theta);
                float y = xy * sinf(theta);

                positions.push_back(x);
                positions.push_back(y);
                positions.push_back(z);

                float u = (float)j / numLongLines;
                float v = (float)i / numLatLines;
                uvs.push_back(u);
                uvs.push_back(v);

                // 法线：注意没有归一化，归一化是在fragmentshader中进行的
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
            }
        }

        // 生成索引
        for (int i = 0; i < numLatLines; i++) {
            for (int j = 0; j < numLongLines; j++) {
                unsigned int first = i * (numLongLines + 1) + j;
                unsigned int second = first + numLongLines + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }

        geometry->mIndicesCount = static_cast<uint32_t>(indices.size());

        // VBO创建
        GLuint& posVbo = geometry->mPosVbo,
            uvVbo = geometry->mUvVao,
            ebo = geometry->mEbo,
            normalVbo = geometry->mNormalVbo;
        // POS
        geometry->glGenBuffers(1, &posVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

        // UV
        geometry->glGenBuffers(1, &uvVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

        // 法线
        geometry->glGenBuffers(1, &normalVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

        // EBO
        geometry->glGenBuffers(1, &ebo);
        geometry->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        geometry->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // VAO创建
        geometry->glGenVertexArrays(1, &geometry->mVao);
        geometry->glBindVertexArray(geometry->mVao);

        // 绑定顶点位置
        geometry->glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        geometry->glEnableVertexAttribArray(0);
        geometry->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // 绑定UV坐标
        geometry->glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        geometry->glEnableVertexAttribArray(1);
        geometry->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

        // 绑定法线
        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glEnableVertexAttribArray(2);
        geometry->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // 绑定EBO
        geometry->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        geometry->glBindVertexArray(0);
        return geometry;
    }

    MyGeometry* MyGeometry::createPlane(float width, float height)
    {
        MyGeometry* geometry = new MyGeometry();

        geometry->mIndicesCount = 6;

        float halfW = width / 2.0f;
        float halfH = height / 2.0f;

        float positions[] = {
            -halfW, -halfH, 0.0f,
             halfW, -halfH, 0.0f,
             halfW,  halfH, 0.0f,
            -halfW,  halfH, 0.0f,
        };

        float uvs[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
        };

        float normals[] = {
         0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 1.0f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // VBO创建
        GLuint& posVbo = geometry->mPosVbo,
            uvVbo = geometry->mUvVao,
            ebo = geometry->mEbo,
            normalVbo = geometry->mNormalVbo;
        //POS
        geometry->glGenBuffers(1, &posVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

        //UV
        geometry->glGenBuffers(1, &uvVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

        // 法线
        geometry->glGenBuffers(1, &normalVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

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
        geometry->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // 绑定UV坐标
        geometry->glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        geometry->glEnableVertexAttribArray(1);
        geometry->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

        // 绑定法线
        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glEnableVertexAttribArray(2);
        geometry->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // 绑定EBO
        geometry->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        geometry->glBindVertexArray(0);
        return geometry;
    }

    MyGeometry* MyGeometry::createLogoQuad(float width, float height)
    {
        MyGeometry* geometry = new MyGeometry();

        geometry->mIndicesCount = 6;

        // 顶点坐标（像素空间，左上角为(0,0)，右下角为(width, height)）
        float positions[] = {
            0.0f,      0.0f,     0.0f, // 左上
            width,     0.0f,     0.0f, // 右上
            width,     height,   0.0f, // 右下
            0.0f,      height,   0.0f, // 左下
        };

        float uvs[] = {
            0.0f, 1.0f, // 左上
            1.0f, 1.0f, // 右上
            1.0f, 0.0f, // 右下
            0.0f, 0.0f, // 左下
        };

        float normals[] = {
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // VBO/EBO/VAO 创建
        GLuint& posVbo = geometry->mPosVbo,
            uvVbo = geometry->mUvVao,
            ebo = geometry->mEbo,
            normalVbo = geometry->mNormalVbo;

        geometry->glGenBuffers(1, &posVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

        geometry->glGenBuffers(1, &uvVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

        geometry->glGenBuffers(1, &normalVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

        geometry->glGenBuffers(1, &ebo);
        geometry->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        geometry->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        geometry->glGenVertexArrays(1, &geometry->mVao);
        geometry->glBindVertexArray(geometry->mVao);

        geometry->glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        geometry->glEnableVertexAttribArray(0);
        geometry->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        geometry->glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        geometry->glEnableVertexAttribArray(1);
        geometry->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glEnableVertexAttribArray(2);
        geometry->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        geometry->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        geometry->glBindVertexArray(0);
        return geometry;
    }
}