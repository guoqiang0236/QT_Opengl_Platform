#pragma once
#include "../pch.h"
#include "MyObject.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "tools/Mytools.h"
#include "mesh/MyinstancedMesh.h"
#include "material/MyphongInstanceMaterial.h"
#include "material/MygrassInstanceMaterial.h"
#include "MyTexture.h"

class MyAssimpInstanceLoader
{
public:
	MyAssimpInstanceLoader();
	~MyAssimpInstanceLoader();

	static MyOpenGL::MyObject* load(const std::string& path, int instanceCount);

private:
	static void processNode(aiNode* ainode, MyOpenGL::MyObject* parent, const aiScene* scene, const std::string& rootPath, int instanceCount);
    static MyOpenGL::MyInstancedMesh* processMesh(aiMesh* aimesh, const aiScene* scene, const std::string& rootPath, int instanceCount);
	static MyOpenGL::MyTexture* processTexture(const aiMaterial* aimat, const aiTextureType& type, const aiScene* scene, const std::string& rootPath);

	static glm::mat4 getMat4f(aiMatrix4x4 value);
};

