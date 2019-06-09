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
/************************************************************************/
/* UFUNCTIONS                                                           */
/************************************************************************/
bool ARndrAssimp::LoadMesh(FString FilePath, FString&ImportError, TArray<FVector>&Section, int32 index)
{
	TArray<TArray<FVector>>Meshes;
	FVector vertice;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(TCHAR_TO_UTF8(*FilePath), aiProcess_Triangulate);
	if (!scene)
	{
		ImportError = importer.GetErrorString();
		return false;
	}
	/************************************************************************/
	/* getting mesh data                                                    */
	/************************************************************************/
	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		for (size_t j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
		{
			vertice.X = scene->mMeshes[i]->mVertices[j].x;
			vertice.Y = scene->mMeshes[i]->mVertices[j].y;
			vertice.Z = scene->mMeshes[i]->mVertices[j].z;
			Section.Push(vertice);
		}
		Meshes.Push(Section);
	}
	/************************************************************************/
	/* returning mesh data                                                  */
	/************************************************************************/
	if (Meshes.IsValidIndex(index))
	{
		Section = Meshes[index];
		return true;
	}
	else
	{
		Section.Empty();
		UE_LOG(LogTemp, Error, TEXT("invalid index"));
		return false;
	}
}

