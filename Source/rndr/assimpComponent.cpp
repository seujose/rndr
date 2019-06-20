// Fill out your copyright notice in the Description page of Project Settings.


#include "assimpComponent.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <vector>

// Sets default values for this component's properties
UassimpComponent::UassimpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


bool UassimpComponent::NewLoadMesh(FString FilePath, TArray<FVector>&vertices, TArray<FVector>&normals, TArray<int32>&faces, TArray<int32>&faceNormals)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(TCHAR_TO_UTF8(*FilePath), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipWindingOrder);

	for (size_t i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
	{
		FVector theVertex, theNormal;
		theVertex.X = scene->mMeshes[0]->mVertices[i].x;
		theVertex.Y = scene->mMeshes[0]->mVertices[i].y;
		theVertex.Z = scene->mMeshes[0]->mVertices[i].z;

		theNormal.X = scene->mMeshes[0]->mNormals[i].x;
		theNormal.Y = scene->mMeshes[0]->mNormals[i].y;
		theNormal.Z = scene->mMeshes[0]->mNormals[i].z;
		vertices.Push(theVertex);
		normals.Push(theNormal);
	}
	for (size_t i = 0; i < scene->mMeshes[0]->mNumFaces; i++)
	{
		faces.Push(scene->mMeshes[0]->mFaces[i].mIndices[2]);
		faces.Push(scene->mMeshes[0]->mFaces[i].mIndices[1]);
		faces.Push(scene->mMeshes[0]->mFaces[i].mIndices[0]);
	}
	for (size_t i = 0; i < normals.Num(); i++)
	{
	}
	return true;

}

// Called when the game starts
void UassimpComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UassimpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

