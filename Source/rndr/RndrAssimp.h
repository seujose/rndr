#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RndrAssimp.generated.h"
using namespace std;

UCLASS()
class RNDR_API ARndrAssimp : public AActor
{
	GENERATED_BODY()
	
public:	
	ARndrAssimp();
	 
	UFUNCTION(BlueprintCallable, Category = "Assimp")
		bool getMeshInfo(FQuat&rotationOut, FVector&positionOut, FVector&scaleOut, FLinearColor&colourOut, TArray<FString>&textPath, TArray<FVector2D>&UV, FString FilePath, TArray<FVector>&vertices, TArray<FVector>&normals, TArray<int32>&faces, TArray<int32>&faceNormals, int32 importSwitch, TArray<FVector2D>&UVTwo);
	
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

};
