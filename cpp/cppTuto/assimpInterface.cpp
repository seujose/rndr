#include <iostream>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimpInterface.h"
using namespace Assimp;

void recursiveProcessNode(aiNode*node, SceneObject targetParent, const aiScene*scene, aiMatrix4x4 accTransform);

void recursiveProcessNode(aiNode*node, SceneObject targetParent, const aiScene*scene, aiMatrix4x4 accTransform)
{
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		//recursiveProcessNode(node->mChildren[i], parent, scene, transform);
	}
}

void assimpInterface::loadModel(string modelPath)
{
	Assimp::Importer importer;
	const aiScene*scene = importer.ReadFile(modelPath, aiProcess_Triangulate);
	if (scene)
	{
		aiMatrix4x4 accTransform;
		SceneObject sceneObject;
		recursiveProcessNode(scene->mRootNode, sceneObject, scene, accTransform);
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
