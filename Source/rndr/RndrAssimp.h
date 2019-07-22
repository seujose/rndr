#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "RndrAssimp.generated.h"

using namespace std;
using namespace Assimp;


USTRUCT(BlueprintType)
struct FMeshInfo
{
	GENERATED_BODY()
		UPROPERTY(BlueprintReadOnly, Category="meshInfo")
		FVector position;
	UPROPERTY(BlueprintReadOnly, Category = "meshInfo")
		FQuat rotation;
	UPROPERTY(BlueprintReadOnly, Category = "meshInfo")
		FVector scale;
	UPROPERTY(BlueprintReadOnly, Category = "meshInfo")
		FString name;
	UPROPERTY(BlueprintReadOnly, Category = "meshInfo")
		FVector parentPosition;
	UPROPERTY(BlueprintReadOnly, Category = "meshInfo")
		FString parentName;
	UPROPERTY(BlueprintReadOnly, Category = "meshInfo")
		TArray<FVector>vertices;
	UPROPERTY(BlueprintReadOnly, Category = "meshInfo")
		TArray<FVector>normals;
	UPROPERTY(BlueprintReadOnly, Category = "meshInfo")
		TArray<int32>faces;
	FMeshInfo()
	{
	};
};


UCLASS()
class RNDR_API ARndrAssimp : public AActor
{
	GENERATED_BODY()
	
public:	
	ARndrAssimp();
	 
	UFUNCTION(BlueprintCallable, Category = "Assimp")
		bool getMeshInfo(FQuat&rotationOut, FVector&positionOut, FVector&scaleOut, FLinearColor&colourOut, TArray<FString>&textPath, TArray<FVector2D>&UV, FString FilePath, TArray<FVector>&vertices, TArray<FVector>&normals, TArray<int32>&faces, TArray<int32>&faceNormals, int32 importSwitch, TArray<FVector2D>&UVTwo);
	UFUNCTION(BlueprintCallable, Category = "assimp")
		bool newMeshInfo(FString path, TArray<FMeshInfo>&meshInfoOut);
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

};
