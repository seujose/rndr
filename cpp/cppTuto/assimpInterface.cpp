#include "pch.h"
#include <iostream>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimpInterface.h"
using namespace Assimp;

void recursiveProcessNode(aiNode*node, SceneObject targetParent, const aiScene*scene, aiMatrix4x4 accTransform);

void recursiveProcessNode(aiNode*node, SceneObject targetParent, const aiScene*scene, aiMatrix4x4 accTransform)
{
	SceneObject parent;
	aiMatrix4x4 transform;
	aiVector3D pos, scl;
	aiQuaternion rot;
	cout <<"node name("<< node->mName.C_Str()<<")"<<endl;
	node->mTransformation.Decompose(scl, rot, pos);
	cout <<"node pos("<< pos.x<<")(" <<pos.y<<")("<<pos.z<<")\n"<< endl; 
	if (node->mNumMeshes>0)
	{
		SceneObject NewObject;
		targetParent.transform = (NewObject.transform);
		parent = NewObject;
	}
	else
	{
		parent = targetParent;
		transform = node->mTransformation*accTransform;
	}


	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		recursiveProcessNode(node->mChildren[i], parent, scene, transform);
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
