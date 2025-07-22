#include "MyMesh.h"

MyMesh::MyMesh(MyGeometry* geometry, MyMaterial* material)
{
	mGeometry = geometry;
	mMaterial = material;
	m_Type = ObjectType::Mesh;
}

MyMesh::~MyMesh()
{
}
