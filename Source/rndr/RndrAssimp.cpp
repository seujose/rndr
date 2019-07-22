#include "RndrAssimp.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <vector>
#include "rndrStream.h"
#include <iostream>
using namespace Assimp;
using namespace std;
rndrStream theStream;


ARndrAssimp::ARndrAssimp()
{
	PrimaryActorTick.bCanEverTick = true;
	std::cout.rdbuf(&theStream);
}
void ARndrAssimp::BeginPlay()
{
	Super::BeginPlay();
}
void ARndrAssimp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void processarNode(aiNode*node, const aiScene*scene, TArray<FMeshInfo>&hahah);
void processarMesh(aiMesh*mesh, const aiScene* scene, FMeshInfo&meshInfo);





bool ARndrAssimp::getMeshInfo(FQuat&rotationOut, FVector&positionOut, FVector&scaleOut, FLinearColor&colourOut, TArray<FString>&textPath, TArray<FVector2D>&UV, FString FilePath, TArray<FVector>&vertices, TArray<FVector>&normals, TArray<int32>&faces, TArray<int32>&faceNormals, int32 importSwitch, TArray<FVector2D>&UVTwo)
{
	Assimp::Importer importer;
	const aiScene*scene=nullptr;
	switch (importSwitch)
	{
	case 1:
	{
		scene = importer.ReadFile(TCHAR_TO_UTF8(*FilePath), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded);
	}
	break;
	case 2:
	{
		scene = importer.ReadFile(TCHAR_TO_UTF8(*FilePath), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	}
	break;
	}
	if (scene!=NULL)
	{
		//#otimizar com auto
		if (scene->HasMeshes())
		{
			aiVector3D assimpScale;
			aiQuaternion assimpRotation;
			aiVector3D assimpPosition; 
			FString tempFString;
			string tempString;

			for (size_t i = 0; i < scene->mRootNode->mNumChildren; i++)
			{
				aiNode*theRootNode;
				theRootNode = scene->mRootNode;
				tempFString = theRootNode->mName.C_Str();
				//UE_LOG(LogTemp, Warning, TEXT("root node (%s)"), *tempFString);
				
				scene->mRootNode->mChildren[i]->mTransformation.Decompose(assimpScale, assimpRotation, assimpPosition);
				
				positionOut.X = assimpPosition.x;
				positionOut.Y = assimpPosition.y;
				positionOut.Z = assimpPosition.z;

				scaleOut.X = assimpScale.x;
				scaleOut.Y = assimpScale.y;
				scaleOut.Z = assimpScale.z;

				rotationOut.W = assimpRotation.w;
				rotationOut.X = assimpRotation.x;
				rotationOut.Y = assimpRotation.y;
				rotationOut.Z = assimpRotation.z;

				//UE_LOG(LogTemp, Warning, TEXT("%s rotation(%s) ocation(%s) scale(%s)"), *FilePath, *rotationOut.ToString(),*positionOut.ToString(), *scaleOut.ToString());
			}
		}
		if (scene->mMeshes[0]->mNumVertices>0)//previne crash lendo fbx de uma luz por exemplo
		{
			for (size_t i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
			{
				FVector theVertex, theNormal;
				theVertex.X = scene->mMeshes[0]->mVertices[i].x;
				theVertex.Y = scene->mMeshes[0]->mVertices[i].y;
				theVertex.Z = scene->mMeshes[0]->mVertices[i].z;
				theNormal.X = scene->mMeshes[0]->mNormals[i].x;
				theNormal.Y = scene->mMeshes[0]->mNormals[i].y;
				theNormal.Z = scene->mMeshes[0]->mNormals[i].z;
				if (importSwitch == 2)
				{
					theVertex.Y = scene->mMeshes[0]->mVertices[i].y*-1;
					theNormal.Y = scene->mMeshes[0]->mNormals[i].y*-1;
				}
				vertices.Push(theVertex);
				normals.Push(theNormal);

				FVector2D UVtemp;
				aiMesh*theMesh = scene->mMeshes[0];
				aiVector3D* textureVec = nullptr;


				if (scene->mMeshes[0]->mTextureCoords[0])
				{
					textureVec = &theMesh->mTextureCoords[0][i];
					UVtemp.X = textureVec->x;
					UVtemp.Y = textureVec->y;
					UV.Add(UVtemp);
				}

				if (scene->mMeshes[0]->mTextureCoords[1])
				{
					textureVec = &theMesh->mTextureCoords[1][i];
					UVtemp.X = textureVec->x;
					UVtemp.Y = textureVec->y;
					UVTwo.Add(UVtemp);
				}
			}
			for (size_t i = 0; i < scene->mMeshes[0]->mNumFaces; i++)
			{
				faces.Push(scene->mMeshes[0]->mFaces[i].mIndices[2]);
				faces.Push(scene->mMeshes[0]->mFaces[i].mIndices[1]);
				faces.Push(scene->mMeshes[0]->mFaces[i].mIndices[0]);
			}
			for (size_t i = 0; i < scene->mNumMaterials; i++)
			{
				const aiMaterial*material = scene->mMaterials[i];
				if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
				{
					aiString path;
					if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
					{
						textPath.Add(path.C_Str());
					}
				}
				else
				{
					aiColor4D assimpColor;
					if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &assimpColor))
					{
						colourOut.R = assimpColor.r;
						colourOut.G = assimpColor.g;
						colourOut.B = assimpColor.b;
						colourOut.A = assimpColor.a;
					}
				}
			}
		}
		return true;
	}
	return false;
}
void processarMesh(aiMesh*mesh, const aiScene* scene, FMeshInfo&meshInfo)
{ 
	FVector tempFVector;
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		tempFVector.X = mesh->mVertices[i].x;
		tempFVector.Y = mesh->mVertices[i].y;
		tempFVector.Z = mesh->mVertices[i].z;
		meshInfo.vertices.Add(tempFVector);

		tempFVector.X = mesh->mNormals[i].x;
		tempFVector.Y = mesh->mNormals[i].y;
		tempFVector.Z = mesh->mNormals[i].z;
		meshInfo.normals.Add(tempFVector);


		
	}
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		meshInfo.faces.Add(mesh->mFaces[i].mIndices[2]);
		meshInfo.faces.Add(mesh->mFaces[i].mIndices[1]);
		meshInfo.faces.Add(mesh->mFaces[i].mIndices[0]);
	}
	if (mesh->HasTextureCoords(0))//#otimizar
	{
		FVector2D tempVector2D;
		for (size_t j = 0; j < mesh->mNumVertices; j++)
		{
			aiVector3D *meshTextureCoord = &mesh->mTextureCoords[0][j];
			tempVector2D.X = meshTextureCoord->x;
			tempVector2D.Y = meshTextureCoord->y;
			meshInfo.uv0.Add(tempVector2D);
		}
	}
	if (mesh->HasTextureCoords(1))//#otimizar
	{
		FVector2D tempVector2D;
		for (size_t j = 0; j < mesh->mNumVertices; j++)
		{
			aiVector3D *meshTextureCoord = &mesh->mTextureCoords[1][j];
			tempVector2D.X = meshTextureCoord->x;
			tempVector2D.Y = meshTextureCoord->y;
			meshInfo.uv1.Add(tempVector2D);
		}
	}
}

void processarNode(aiNode*node, const aiScene*scene, TArray<FMeshInfo>&meshInfo)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		FMeshInfo tempMeshInfo;
		aiVector3D aiPosition, aiScale, parentPos, parentScale;
		aiQuaternion aiRotation, parentRotation;
		aiMatrix4x4 nodeTransform = node->mParent->mTransformation;
		if (node->mParent->mParent!=NULL)
		{
			aiMatrix4x4 nodeTransformParent = node->mParent->mParent->mTransformation;
			nodeTransformParent.Decompose(parentScale, parentRotation, parentPos);
			tempMeshInfo.parentPosition.Set(parentPos.x, parentPos.y, parentPos.z);
			tempMeshInfo.parentName = node->mParent->mParent->mName.C_Str();
		}
		nodeTransform.Decompose(aiScale, aiRotation, aiPosition);
		tempMeshInfo.position.Set(aiPosition.x, aiPosition.y, aiPosition.z);
		tempMeshInfo.scale.Set(aiScale.x, aiScale.y, aiScale.z);
		tempMeshInfo.rotation.W = aiRotation.w;
		tempMeshInfo.rotation.X = aiRotation.x;
		tempMeshInfo.rotation.Y = aiRotation.y;
		tempMeshInfo.rotation.Z = aiRotation.z;
		tempMeshInfo.name = node->mName.C_Str();
		/*cout << "***************************\nnode name(" << node->mName.C_Str() << ")\n" << 
		"node pos(" << aiPosition.x << ")" << "(" << aiPosition.y << ")" << "(" << aiPosition.z << ")\n"<< 
		"node rot(" << aiRotation.x << ")" << "(" << aiRotation.y << ")" << "(" << aiRotation.z << ")\n" << 
		"node scale(" << aiScale.x << ")" << "(" << aiScale.y << ")" << "(" << aiScale.z << ")" << endl;*/
		aiMesh*mesh = scene->mMeshes[node->mMeshes[i]];
		processarMesh(mesh, scene, tempMeshInfo);
		meshInfo.Add(tempMeshInfo);
	}
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		processarNode(node->mChildren[i], scene, meshInfo);
	}
}
bool ARndrAssimp::newMeshInfo(FString path, TArray<FMeshInfo>&meshInfo)
{
	Assimp::Importer importer;
	const aiScene*scene = nullptr;
	scene = importer.ReadFile(TCHAR_TO_UTF8(*path), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded);
	if (scene != NULL)
	{
		processarNode(scene->mRootNode, scene, meshInfo);
		return true;
	}
	return false;
}

