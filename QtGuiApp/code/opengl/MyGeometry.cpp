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

        // VBO����
        //POS
        glGenBuffers(1, &mPosVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

        //UV
        glGenBuffers(1, &mUvVao);
        glBindBuffer(GL_ARRAY_BUFFER, mUvVao);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

        // ����
        glGenBuffers(1, &mNormalVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

        //EBO
        glGenBuffers(1, &mEbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // VAO����
        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);

        // �󶨶���λ��
        glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // ��UV����
        glBindBuffer(GL_ARRAY_BUFFER, mUvVao);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

        // �󶨷���
        glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // ��EBO
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
        // �����嶥�����ݣ��߳� = 2 * halfSize��
        const float halfSize = 0.5f; // �������߳�

        geometry->mIndicesCount = 36; // ��������6���棬ÿ����2�������Σ�ÿ��������3�����㣬�ܹ�6*2*3=36������

        // ����λ������ (24�����㣬ÿ����4������)
        float positions[] = {
            // ǰ�� (Front)
            -halfSize, -halfSize,  halfSize, // ����
             halfSize, -halfSize,  halfSize, // ����
             halfSize,  halfSize,  halfSize, // ����
            -halfSize,  halfSize,  halfSize, // ����

            // ���� (Back)
             halfSize, -halfSize, -halfSize, // ����
            -halfSize, -halfSize, -halfSize, // ����
            -halfSize,  halfSize, -halfSize, // ����
             halfSize,  halfSize, -halfSize, // ����

             // ���� (Left)
             -halfSize, -halfSize, -halfSize, // ����
             -halfSize, -halfSize,  halfSize, // ����
             -halfSize,  halfSize,  halfSize, // ����
             -halfSize,  halfSize, -halfSize, // ����

             // ���� (Right)
              halfSize, -halfSize,  halfSize, // ����
              halfSize, -halfSize, -halfSize, // ����
              halfSize,  halfSize, -halfSize, // ����
              halfSize,  halfSize,  halfSize, // ����

              // ���� (Top)
              -halfSize,  halfSize,  halfSize, // ����
               halfSize,  halfSize,  halfSize, // ����
               halfSize,  halfSize, -halfSize, // ����
              -halfSize,  halfSize, -halfSize, // ����

              // ���� (Bottom)
              -halfSize, -halfSize, -halfSize, // ����
               halfSize, -halfSize, -halfSize, // ����
               halfSize, -halfSize,  halfSize, // ����
              -halfSize, -halfSize,  halfSize  // ����
        };

        // UV�������� (ÿ����4��UV����)
        float uvs[] = {
            // ÿ�����UV����˳��: ����(0,0), ����(1,0), ����(1,1), ����(0,1)
            0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f, // Front
            0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f, // Back
            0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f, // Left
            0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f, // Right
            0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f, // Top
            0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f  // Bottom
        };

        // �������� (36��������ÿ����6������)
        unsigned int indices[] = {
            // ǰ��
            0, 1, 2,  2, 3, 0,
            // ����
            4, 5, 6,  6, 7, 4,
            // ����
            8, 9, 10,  10, 11, 8,
            // ����
            12, 13, 14,  14, 15, 12,
            // ����
            16, 17, 18,  18, 19, 16,
            // ����
            20, 21, 22,  22, 23, 20
        };

        //����
        float normals[] = {
            // ǰ�� (Front)
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            // ���� (Back)
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,

            // ���� (Left)
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,

            // ���� (Right)
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            // ���� (Top)
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // ���� (Bottom)
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f
        };


        // VBO����
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

        // ����
        geometry->glGenBuffers(1, &normalVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

        //EBO
        geometry->glGenBuffers(1, &ebo);
        geometry->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        geometry->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // VAO����
        geometry->glGenVertexArrays(1, &geometry->mVao);
        geometry->glBindVertexArray(geometry->mVao);

        // �󶨶���λ��
        geometry->glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        geometry->glEnableVertexAttribArray(0);
        geometry->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // ��UV����
        geometry->glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        geometry->glEnableVertexAttribArray(1);
        geometry->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

        // �󶨷���
        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glEnableVertexAttribArray(2);
        geometry->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // ��EBO
        geometry->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        geometry->glBindVertexArray(0);
        return geometry;
    }

    MyGeometry* MyGeometry::createSphere(float size)
    {
        MyGeometry* geometry = new MyGeometry();

        // ����γ���뾭�ߵ�����
        const int numLatLines = 60;  // γ��
        const int numLongLines = 60; // ����
        const float radius = size * 0.5f;

        std::vector<float> positions;
        std::vector<float> uvs;
        std::vector<float> normals;
        std::vector<unsigned int> indices;

        // ͨ������ѭ����γ�����⣬�������ڣ�->λ�á�uv
        for (int i = 0; i <= numLatLines; i++) {
            float phi = i * glm::pi<float>() / numLatLines;  // γ�� [0, pi]
            float xy = radius * sinf(phi);
            float z = radius * cosf(phi);

            for (int j = 0; j <= numLongLines; j++) {
                float theta = j * 2 * glm::pi<float>() / numLongLines;  // ���� [0, 2pi]
                float x = xy * cosf(theta);
                float y = xy * sinf(theta);

                positions.push_back(x);
                positions.push_back(y);
                positions.push_back(z);

                float u = (float)j / numLongLines;
                float v = (float)i / numLatLines;
                uvs.push_back(u);
                uvs.push_back(v);

                // ���ߣ�ע��û�й�һ������һ������fragmentshader�н��е�
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
            }
        }

        // ��������
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

        // VBO����
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

        // ����
        geometry->glGenBuffers(1, &normalVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

        // EBO
        geometry->glGenBuffers(1, &ebo);
        geometry->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        geometry->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // VAO����
        geometry->glGenVertexArrays(1, &geometry->mVao);
        geometry->glBindVertexArray(geometry->mVao);

        // �󶨶���λ��
        geometry->glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        geometry->glEnableVertexAttribArray(0);
        geometry->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // ��UV����
        geometry->glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        geometry->glEnableVertexAttribArray(1);
        geometry->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

        // �󶨷���
        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glEnableVertexAttribArray(2);
        geometry->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // ��EBO
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

        // VBO����
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

        // ����
        geometry->glGenBuffers(1, &normalVbo);
        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

        //EBO
        geometry->glGenBuffers(1, &ebo);
        geometry->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        geometry->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // VAO����
        geometry->glGenVertexArrays(1, &geometry->mVao);
        geometry->glBindVertexArray(geometry->mVao);

        // �󶨶���λ��
        geometry->glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        geometry->glEnableVertexAttribArray(0);
        geometry->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // ��UV����
        geometry->glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        geometry->glEnableVertexAttribArray(1);
        geometry->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

        // �󶨷���
        geometry->glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        geometry->glEnableVertexAttribArray(2);
        geometry->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // ��EBO
        geometry->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        geometry->glBindVertexArray(0);
        return geometry;
    }

    MyGeometry* MyGeometry::createLogoQuad(float width, float height)
    {
        MyGeometry* geometry = new MyGeometry();

        geometry->mIndicesCount = 6;

        // �������꣨���ؿռ䣬���Ͻ�Ϊ(0,0)�����½�Ϊ(width, height)��
        float positions[] = {
            0.0f,      0.0f,     0.0f, // ����
            width,     0.0f,     0.0f, // ����
            width,     height,   0.0f, // ����
            0.0f,      height,   0.0f, // ����
        };

        float uvs[] = {
            0.0f, 1.0f, // ����
            1.0f, 1.0f, // ����
            1.0f, 0.0f, // ����
            0.0f, 0.0f, // ����
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

        // VBO/EBO/VAO ����
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