#include "assimpComponent.h"
#include <iostream>
#include "vrayInterface.h"
using namespace std;
UassimpComponent::UassimpComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UassimpComponent::openMesh(FString path, int32& SectionCount, FString& ErrorCode)
{
	Assimp::Importer importer;
	std::string filename(TCHAR_TO_UTF8(*path));
	const aiScene* scene = importer.ReadFile(filename,aiProcess_Triangulate);
	if (!scene)
	{
		ErrorCode = importer.GetErrorString();
		return false;
	}
	_meshCurrentlyProcessed = 0;
	processNode(scene->mRootNode, scene);
	//scene->mRootNode->mTransformation();
	SectionCount = _meshCurrentlyProcessed;
	return true;
}

bool UassimpComponent::getSection(int32 index, TArray<FVector>& Vertices, TArray<int32>& Faces, TArray<FVector>& Normals, TArray<FVector2D>& UV, TArray<FVector>& Tangents, TArray<FVector>&Transform)
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
	Transform = _transform;
	return true;
}

void UassimpComponent::clear()
{
	_vertices.Empty();
	_indices.Empty();
	_normals.Empty();
	_uvs.Empty();
	_tangents.Empty();
	_vertexColors.Empty();
	_meshCurrentlyProcessed = 0;
}

// Called when the game starts
void UassimpComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UassimpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UassimpComponent::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// the very first time this method runs, we'll need to create the empty arrays
	// we can't really do that in the class constructor because we don't know how many meshes we'll read, and this data can change between imports
	if (_vertices.Num() <= _meshCurrentlyProcessed) 
	{
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

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
	{
		FVector vertex, normal;
		// process vertex positions, normals and UVs
		vertex.X = mesh->mVertices[i].x;
		vertex.Y = mesh->mVertices[i].y;
		vertex.Z = mesh->mVertices[i].z;
		normal.X = mesh->mNormals[i].x;
		normal.Y = mesh->mNormals[i].y;
		normal.Z = mesh->mNormals[i].z;
		
		// if the mesh contains tex coords
		//int uvIndex = 0;
		if (mesh->mTextureCoords[uvIndex]) 
		{
			FVector2D uvs;
			uvs.X = mesh->mTextureCoords[uvIndex][i].x;
			uvs.Y = mesh->mTextureCoords[uvIndex][i].y;
			_uvs[_meshCurrentlyProcessed].Add(uvs);
		}
		else 
		{
			_uvs[_meshCurrentlyProcessed].Add(FVector2D(0.f, 0.f));
		}
		_vertices[_meshCurrentlyProcessed].Add(vertex);
		_normals[_meshCurrentlyProcessed].Add(normal);
	}

	if (_requiresFullRecreation) 
	{
		// process indices
		for (uint32 i = 0; i < mesh->mNumFaces; i++) 
		{
			aiFace face = mesh->mFaces[i];
			_indices[_meshCurrentlyProcessed].Add(face.mIndices[2]);
			_indices[_meshCurrentlyProcessed].Add(face.mIndices[1]);
			_indices[_meshCurrentlyProcessed].Add(face.mIndices[0]);
		}
	}
}

void UassimpComponent::processNode(aiNode* node, const aiScene* scene)
{
	aiNode* children = *scene->mRootNode->mChildren;
	aiMatrix4x4 childrenTransform = children->mTransformation;
	FVector position;
	FVector scale;
	FVector rotation;
	position.ZeroVector;
	scale.ZeroVector;
	rotation.ZeroVector;
	_transform.SetNumZeroed(3, true);
	aiVector3D pos;
	aiVector3D rot;
	aiVector3D scl;
	childrenTransform.Decompose(scl, rot, pos);
	position.X = pos.x;
	position.Y = pos.y;
	position.Z = pos.z;
	scale.X = scl.x;
	scale.Y = scl.y;
	scale.Z = scl.z;
	//scale = scale * 10;
	rotation.X = rot.x;
	rotation.Y = rot.y;
	rotation.Z = rot.z;
	_transform[0].Set(scale.X, scale.Y, scale.Z);
	_transform[1].Set(rotation.X, rotation.Y, rotation.Z);
	_transform[2].Set(position.X, position.Y*-1, position.Z*-1);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	for (uint32 i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
		++_meshCurrentlyProcessed;
	}
	uint32 nodes = node->mNumMeshes;
	// do the same for all of its children
	for (uint32 i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

