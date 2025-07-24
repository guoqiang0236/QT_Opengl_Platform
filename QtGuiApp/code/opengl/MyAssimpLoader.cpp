#include "MyAssimpLoader.h"

MyAssimpLoader::MyAssimpLoader()
{
}

MyAssimpLoader::~MyAssimpLoader()
{
}

MyOpenGL::MyObject* MyAssimpLoader::load(const std::string& path) {
	//�ó�ģ������Ŀ¼
	std::size_t lastIndex = path.find_last_of("//");
	auto rootPath = path.substr(0, lastIndex + 1);


	MyOpenGL::MyObject* rootNode = new MyOpenGL::MyObject();
	Assimp::Importer importer; //������
	//aiProcess_Triangulate ��ģ���е��ı��������λ�
	//aiProcess_GenNormals  ���ģ��û�з��������ɷ���
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

	//��֤��ȡ�Ƿ���ȷ˳��
	//AI_SCENE_FLAGS_INCOMPLETE �ļ�������
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Error: Model Read Failed!" << std::endl;
		return nullptr;
	}

	processNode(scene->mRootNode, rootNode, scene, rootPath);

	return rootNode;
}

//����ڵ�
//aiNode������children����Ϣ,Ҳ��mesh�ı��
//ÿ���ڵ��Ӧһ��object
void MyAssimpLoader::processNode(aiNode* ainode, MyOpenGL::MyObject* parent, const aiScene* scene, const std::string& rootPath) {
	MyOpenGL::MyObject* node = new MyOpenGL::MyObject();
	parent->addChild(node);//����Object�ĸ��ӹ�ϵ

	glm::mat4 localMatrix = getMat4f(ainode->mTransformation);
	//λ�� ��ת ����
	glm::vec3 position, eulerAngle, scale;
	MyOpenGL::MyTools::decompose(localMatrix, position, eulerAngle, scale);
	node->setPosition(position);
	node->setAngleX(eulerAngle.x);
	node->setAngleY(eulerAngle.y);
	node->setAngleZ(eulerAngle.z);
	node->setScale(scale);

	//�����û��mesh���ҽ���
	for (int i = 0; i < ainode->mNumMeshes; i++)
	{
		int meshID = ainode->mMeshes[i]; //����ֻ��֪��ID Ҫ�ҵ�mesh�Ķ��󻹵�ȥ����aiScene����
		aiMesh* aiMesh = scene->mMeshes[meshID];//ainodeֻ�Ǵ��ID scene������Ǵ�Ķ���
		MyOpenGL::MyMesh* mesh = processMesh(aiMesh, scene, rootPath);
		node->addChild(mesh); //object���治ֻ��ҽڵ�node �����mesh
	}

	for (int i = 0; i < ainode->mNumChildren; i++)
	{
		processNode(ainode->mChildren[i], node, scene, rootPath);
	}

}

////����mesh
MyOpenGL::MyMesh* MyAssimpLoader::processMesh(aiMesh* aimesh, const aiScene* scene, const std::string& rootPath) {
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> uvs;
	std::vector<unsigned int> indices;

	for (int i = 0; i < aimesh->mNumVertices; i++)
	{
		//��i�������λ��
		positions.push_back(aimesh->mVertices[i].x);
		positions.push_back(aimesh->mVertices[i].y);
		positions.push_back(aimesh->mVertices[i].z);

		//��i������ķ���
		normals.push_back(aimesh->mNormals[i].x);
		normals.push_back(aimesh->mNormals[i].y);
		normals.push_back(aimesh->mNormals[i].z);

		//��i�������uv
		//һ��mesh������n��uv(diffuse��ͼuv,������ͼuv...)
		//һ���ע��0��uv,һ������µ�0��uv����ͼuv
		if (aimesh->mTextureCoords[0]) //������ڵ�0��uv
		{
			uvs.push_back(aimesh->mTextureCoords[0][i].x); //u
			uvs.push_back(aimesh->mTextureCoords[0][i].y); //v
		}
		else
		{
			//��������ڵ�0��uv ������ΪĬ��ֵ
			uvs.push_back(0.0f);
			uvs.push_back(0.0f);
		}
	}

	//����mesh�е�����ֵ
	//ÿ�������ξ���һ��aiFace
	//����ÿһ��������(face) -> ����ÿһ��indices
	for (int i = 0; i < aimesh->mNumFaces; i++)
	{
		aiFace face = aimesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	auto geometry = new MyOpenGL::MyGeometry(positions, normals, uvs, indices);
	auto material = new MyOpenGL::MyPhongMaterial();
	//material->mDepthWrite = false;
	//���������ȡ
	//if (aimesh->mMaterialIndex >= 0)
	//{
	//	MyOpenGL::MyTexture* texture = nullptr;
	//	aiMaterial* aiMat = scene->mMaterials[aimesh->mMaterialIndex];
	//	//1 ��ȡ��diffuse��ͼ
	//	texture = processTexture(aiMat, aiTextureType_DIFFUSE, scene, rootPath);
	//	if (texture == nullptr)
	//	{
	//		texture = MyOpenGL::MyTexture::createTexture("assets/textures/defaultTexture.jpg", 0);
	//	}
	//	texture->setUnit(0);
	//	material->mDiffuse = texture;
	//	//2 ��ȡspecular��ͼ
	//	auto speclarMask = processTexture(aiMat, aiTextureType_SPECULAR, scene, rootPath);
	//	if (speclarMask == nullptr)
	//	{
	//		speclarMask = MyOpenGL::MyTexture::createTexture("assets/textures/defaultTexture.jpg", 0);
	//	}
	//	speclarMask->setUnit(1);
	//	material->mSpecularMask = speclarMask;
	//}
	//else
	//{
	//	material->mDiffuse = Texture::createTexture("assets/textures/defaultTexture.jpg", 0);
	//}
	material->mDiffuse = new MyOpenGL::MyTexture("assets/textures/defaultTexture.jpg", 0);
	return new MyOpenGL::MyMesh(geometry, material);
}


//MyOpenGL::MyTexture* MyAssimpLoader::processTexture(const aiMaterial* aimat, const aiTextureType& type, const aiScene* scene, const std::string& rootPath) {
//	MyOpenGL::MyTexture* texture = nullptr;
//	//��ȡͼƬ��ȡ·��
//	aiString aipath;
//	aimat->Get(AI_MATKEY_TEXTURE(type, 0), aipath); //��ȡdiffuse��ͼ�ĵ�0��
//	if (!aipath.length)
//	{
//		return nullptr;
//	}
//
//	//�ж��Ƿ���Ƕ��fbx��ͼƬ
//	const aiTexture* aitexture = scene->GetEmbeddedTexture(aipath.C_Str());//�жϵ�ǰ·����û��Ƕ���������֮��Ӧ
//	if (aitexture)
//	{
//		//����ͼƬ����Ƕ��
//		unsigned char* dataIn = reinterpret_cast<unsigned char*>(aitexture->pcData); // reinterpret_cast��ǿ�Ƶ�����ת��
//		uint32_t widthIn = aitexture->mWidth; //ͨ�������(png, jpg),����������ͼƬ�Ĵ�С(height��0)
//		uint32_t heightIn = aitexture->mHeight;
//		texture = MyOpenGL::MyTexture::createTextureFromMemory(aipath.C_Str(), 0, dataIn, widthIn, heightIn);
//	}
//	else
//	{
//		//����ͼƬ��Ӳ����
//		std::string fullPath = rootPath + aipath.C_Str();
//		texture = MyOpenGL::MyTexture::createTexture(fullPath, 0);
//	}
//	return texture;
//}
//

//��Assimp��aiMatrixת��ΪopenGL��glmat4
glm::mat4 MyAssimpLoader::getMat4f(aiMatrix4x4 value) {
	glm::mat4 to(
		value.a1, value.a2, value.a3, value.a4,
		value.b1, value.b2, value.b3, value.b4,
		value.c1, value.c2, value.c3, value.c4,
		value.d1, value.d2, value.d3, value.d4
	);

	return to;
}