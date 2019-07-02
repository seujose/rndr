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

bool ARndrAssimp::getMeshInfo(TArray<FVector>&UV, FString FilePath, TArray<FVector>&vertices, TArray<FVector>&normals, TArray<int32>&faces, TArray<int32>&faceNormals)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(TCHAR_TO_UTF8(*FilePath),  aiProcess_Triangulate| aiProcess_JoinIdenticalVertices|aiProcess_FlipWindingOrder);
	if (scene!=NULL)
	{
		for (size_t i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
		{
			FVector theVertex, theNormal, theUV;
			
			theVertex.X = scene->mMeshes[0]->mVertices[i].x;
			theVertex.Y = scene->mMeshes[0]->mVertices[i].y;
			theVertex.Z = scene->mMeshes[0]->mVertices[i].z;

			theNormal.X = scene->mMeshes[0]->mNormals[i].x;
			theNormal.Y = scene->mMeshes[0]->mNormals[i].y;
			theNormal.Z = scene->mMeshes[0]->mNormals[i].z;
			vertices.Push(theVertex);
			normals.Push(theNormal);

			aiMesh*theMesh = scene->mMeshes[0];
			aiVector3D* textureVec = &theMesh->mTextureCoords[0][i];
			theUV.X = textureVec->x;
			theUV.Y = textureVec->y;
			theUV.Z = (0.0, 0.0, 0.0);
			UV.Add(theUV);
		}
		 
		for (size_t i = 0; i < scene->mMeshes[0]->mNumFaces; i++)
		{
			faces.Push(scene->mMeshes[0]->mFaces[i].mIndices[2]);
			faces.Push(scene->mMeshes[0]->mFaces[i].mIndices[1]);
			faces.Push(scene->mMeshes[0]->mFaces[i].mIndices[0]);
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
