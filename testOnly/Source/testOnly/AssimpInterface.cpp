// Fill out your copyright notice in the Description page of Project Settings.


#include "AssimpInterface.h"

// Sets default values
AAssimpInterface::AAssimpInterface()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAssimpInterface::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAssimpInterface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AAssimpInterface::openMesh(FString path, int32& SectionCount, FString& ErrorCode)
{
	Assimp::Importer importer;
	std::string filename(TCHAR_TO_UTF8(*path));
	const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene)
	{
		ErrorCode = importer.GetErrorString();
		return false;
	}
	_meshCurrentlyProcessed = 0;
	processNode(scene->mRootNode, scene);
	SectionCount = _meshCurrentlyProcessed;
	return true;
}

bool AAssimpInterface::getSection(int32 index, TArray<FVector>& Vertices, TArray<int32>& Faces, TArray<FVector>& Normals, TArray<FVector2D>& UV, TArray<FVector>& Tangents)
{
	if (index >= _meshCurrentlyProcessed)
	{
		return false;
	}
	Vertices = _vertices[index];
	Faces = _indices[index];
	Normals = _normals[index];
	UV = _uvs[index];
	Tangents = _tangents[index];
	return true;
}

void AAssimpInterface::clear()
{
	_vertices.Empty();
	_indices.Empty();
	_normals.Empty();
	_uvs.Empty();
	_tangents.Empty();
	_vertexColors.Empty();
	_meshCurrentlyProcessed = 0;
}

void AAssimpInterface::processNode(aiNode* node, const aiScene* scene)
{
	for (uint32 i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
		++_meshCurrentlyProcessed;
	}
	uint32 nodes = node->mNumMeshes;
	// do the same for all of its children
	for (uint32 i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

void AAssimpInterface::processMesh(aiMesh* mesh, const aiScene* scene)
{

	// the very first time this method runs, we'll need to create the empty arrays
	// we can't really do that in the class constructor because we don't know how many meshes we'll read, and this data can change between imports
	if (_vertices.Num() <= _meshCurrentlyProcessed) {
		_vertices.AddZeroed();
		_normals.AddZeroed();
		_uvs.AddZeroed();
		_tangents.AddZeroed();
		_vertexColors.AddZeroed();
		_indices.AddZeroed();
	}

	// we check whether the current data to read has a different amount of vertices compared to the last time we generated the mesh
	// if so, it means we'll need to recreate the mesh and resupply new indices.
	if (mesh->mNumVertices != _vertices[_meshCurrentlyProcessed].Num())
		_requiresFullRecreation = true;

	// we reinitialize the arrays for the new data we're reading
	_vertices[_meshCurrentlyProcessed].Empty();
	_normals[_meshCurrentlyProcessed].Empty();
	_uvs[_meshCurrentlyProcessed].Empty();
	// this if actually seems useless, seeing what it does without it
	//if (_requiresFullRecreation) {
	_tangents[_meshCurrentlyProcessed].Empty();
	_vertexColors[_meshCurrentlyProcessed].Empty();
	_indices[_meshCurrentlyProcessed].Empty();
	//}

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		FVector vertex, normal;
		// process vertex positions, normals and UVs
		vertex.X = mesh->mVertices[i].x;
		vertex.Y = mesh->mVertices[i].y;
		vertex.Z = mesh->mVertices[i].z;

		normal.X = mesh->mNormals[i].x;
		normal.Y = mesh->mNormals[i].y;
		normal.Z = mesh->mNormals[i].z;

		// if the mesh contains tex coords
		if (mesh->mTextureCoords[0]) {
			FVector2D uvs;
			uvs.X = mesh->mTextureCoords[0][i].x;
			uvs.Y = mesh->mTextureCoords[0][i].y;
			_uvs[_meshCurrentlyProcessed].Add(uvs);
		}
		else {
			_uvs[_meshCurrentlyProcessed].Add(FVector2D(0.f, 0.f));
		}
		_vertices[_meshCurrentlyProcessed].Add(vertex);
		_normals[_meshCurrentlyProcessed].Add(normal);
	}

	if (_requiresFullRecreation) {
		// process indices
		for (uint32 i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			_indices[_meshCurrentlyProcessed].Add(face.mIndices[2]);
			_indices[_meshCurrentlyProcessed].Add(face.mIndices[1]);
			_indices[_meshCurrentlyProcessed].Add(face.mIndices[0]);
		}
	}
}

