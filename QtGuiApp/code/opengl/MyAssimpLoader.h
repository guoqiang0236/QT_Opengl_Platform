#pragma once
#include "../pch.h"
#include "MyObject.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "tools/Mytools.h"
#include "mesh/MyMesh.h"
#include "material/MyPhongMaterial.h"
#include "MyTexture.h"

class MyAssimpLoader
{
public:
	MyAssimpLoader();
	~MyAssimpLoader();

	static MyOpenGL::MyObject* load(const std::string& path);

private:
	static void processNode(aiNode* ainode, MyOpenGL::MyObject* parent, const aiScene* scene, const std::string& rootPath);
    static MyOpenGL::MyMesh* processMesh(aiMesh* aimesh, const aiScene* scene, const std::string& rootPath);
	static MyOpenGL::MyTexture* processTexture(const aiMaterial* aimat, const aiTextureType& type, const aiScene* scene, const std::string& rootPath);

	static glm::mat4 getMat4f(aiMatrix4x4 value);
};

