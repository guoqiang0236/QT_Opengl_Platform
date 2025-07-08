#include "MyMesh.h"

MyMesh::MyMesh(MyGeometry* geometry, MyMaterial* material)
{
	mGeometry = geometry;
	mMaterial = material;
}

MyMesh::~MyMesh()
{
}
