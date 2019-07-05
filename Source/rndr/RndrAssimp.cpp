#include "RndrAssimp.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <vector>
using namespace Assimp;
ARndrAssimp::ARndrAssimp()
{
	PrimaryActorTick.bCanEverTick = true;
}
void ARndrAssimp::BeginPlay()
{
	Super::BeginPlay();
}
void ARndrAssimp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
/************************************************************************/
/* forward declarations                                                 */
/************************************************************************/

/************************************************************************/
/* implementations                                                      */
/************************************************************************/

bool ARndrAssimp::getMeshInfo(TArray<FString>&textPath, TArray<FVector2D>&UV, FString FilePath, TArray<FVector>&vertices, TArray<FVector>&normals, TArray<int32>&faces, TArray<int32>&faceNormals, int32 importSwitch, TArray<FVector2D>&UVTwo)
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
		UE_LOG(LogTemp, Warning, TEXT("mesh (%s)-> uv count (%d) "),*FilePath , scene->mMeshes[0]->GetNumUVChannels());
		for (size_t i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
		{
			FVector theVertex, theNormal;
			FVector2D UVtemp;
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
			
			//UVs
			//UV diffuse
			aiMesh*theMesh = scene->mMeshes[0];
			aiVector3D* textureVec = &theMesh->mTextureCoords[0][i];
			UVtemp.X = textureVec->x;
			UVtemp.Y = textureVec->y;
			UV.Add(UVtemp);
			//UV lightmap
			if (&theMesh->mTextureCoords[1]!=NULL)
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
			if (material->GetTextureCount(aiTextureType_DIFFUSE)>0)
			{
				aiString path;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL)==AI_SUCCESS)
				{
					textPath.Add(path.C_Str());
				}
			}
		}
		UE_LOG(LogTemp, Display, TEXT("assimp scene is valid %s"), *FilePath);
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("assimp scene is invalid %s"), *FilePath);
	return false;
}


/************************************************************************/
/* UFUNCTIONS                                                           */
/************************************************************************/
