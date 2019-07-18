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

bool ARndrAssimp::getMeshInfo(TArray<FVector>&transformOut, FLinearColor&colourOut, TArray<FString>&textPath, TArray<FVector2D>&UV, FString FilePath, TArray<FVector>&vertices, TArray<FVector>&normals, TArray<int32>&faces, TArray<int32>&faceNormals, int32 importSwitch, TArray<FVector2D>&UVTwo)
{
	
	Assimp::Importer importer;
	const aiScene*scene=nullptr;
	switch (importSwitch)
	{
	case 1:
	{
		scene = importer.ReadFile(TCHAR_TO_UTF8(*FilePath), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded|aiProcess_PreTransformVertices);

		aiMatrix4x4 sceneT = scene->mRootNode->mTransformation;

		aiVector3D theScale;
		aiVector3D theRotation;
		aiVector3D thePosition;

		//sceneT.Transpose();
		sceneT.Decompose(theScale, theRotation, thePosition);
		transformOut.Empty();

		FVector outScale;
		FVector outRotation;
		FVector outPosition;

		outScale.X = theScale.x;
		outScale.Y = theScale.y;
		outScale.Z = theScale.z;
		transformOut.EmplaceAt(0, outScale);

		outRotation.X = theRotation.x;
		outRotation.Y = theRotation.y;
		outRotation.Z = theRotation.z;
		transformOut.EmplaceAt(1, outRotation);

		outPosition.X = thePosition.x;
		outPosition.Y = thePosition.y;
		outPosition.Z = thePosition.z;
		transformOut.EmplaceAt(2, outPosition);
	}
	break;
		
	case 2:
	{
		scene = importer.ReadFile(TCHAR_TO_UTF8(*FilePath), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	}
	break;
	}
	

	


	if (scene!=NULL)
	{//#otimizar com auto
		//UE_LOG(LogTemp, Warning, TEXT("mesh (%s)-> uv count (%d) "),*FilePath , scene->mMeshes[0]->GetNumUVChannels());
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


/************************************************************************/
/* UFUNCTIONS                                                           */
/************************************************************************/
