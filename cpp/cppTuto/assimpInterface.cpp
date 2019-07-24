#include "pch.h"
#include <iostream>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimpInterface.h"
using namespace Assimp;

void processNode(const aiScene*scene, aiNode*node, aiMatrix4x4 accTransform);

void processNode(const aiScene*scene, aiNode*node, aiMatrix4x4 accTransform)
{
	if (node->mNumMeshes>0)
	{
	}
	else
	{

	}
	cout << scene->mNumMeshes << endl;
	cout << "cena processada" << endl;
}

void assimpInterface::loadModel(string modelPath)
{
	Assimp::Importer importer;
	const aiScene*scene = importer.ReadFile(modelPath, aiProcess_Triangulate);
	if (scene)
	{
		aiMatrix4x4 accTransform;
		processNode(scene, scene->mRootNode, accTransform);
	}
	else
	{
		cout << "cena NAO processada" << endl;
	}

}

assimpInterface::assimpInterface()
{
}


assimpInterface::~assimpInterface()
{
}
