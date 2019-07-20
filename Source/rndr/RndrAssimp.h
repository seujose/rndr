#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "RndrAssimp.generated.h"


using namespace std;
using namespace Assimp;
UCLASS()
class RNDR_API ARndrAssimp : public AActor
{
	GENERATED_BODY()
	
public:	
	ARndrAssimp();
	 
	UFUNCTION(BlueprintCallable, Category = "Assimp")
		bool getMeshInfo(FQuat&rotationOut, FVector&positionOut, FVector&scaleOut, FLinearColor&colourOut, TArray<FString>&textPath, TArray<FVector2D>&UV, FString FilePath, TArray<FVector>&vertices, TArray<FVector>&normals, TArray<int32>&faces, TArray<int32>&faceNormals, int32 importSwitch, TArray<FVector2D>&UVTwo);

	UFUNCTION(BlueprintCallable, Category = "Assimp")
		bool openMesh(FString path, int32& SectionCount, FString& ErrorCode);

	UFUNCTION(BlueprintCallable, Category = "Assimp")
		bool getSection(FQuat&rotationOut, FVector&positionOut, FVector&scaleOut, FLinearColor&colourOut,int32 index, TArray<FVector>& Vertices, TArray<int32>& Faces, TArray<FVector>& Normals, TArray<FVector2D>& UV, TArray<FVector>& Tangents);

	UFUNCTION(BlueprintCallable, Category = "Assimp")
		void clear();

private:
	int32 _selectedVertex;
	int32 _meshCurrentlyProcessed;
	bool _addModifier;
	int _lastModifiedTime;
	bool _requiresFullRecreation;

	TArray<TArray<FVector>> _vertices;
	TArray<TArray<int32>> _indices;
	TArray<TArray<FVector>> _normals;
	TArray<TArray<FVector2D>> _uvs;
	TArray<TArray<FVector>> _tangents;
	TArray<TArray<FColor>> _vertexColors;
	FLinearColor _color;

	void processMesh(aiMesh* mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene);

	
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

};
