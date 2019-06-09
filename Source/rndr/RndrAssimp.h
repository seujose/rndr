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
		bool LoadMesh(FString FilePath, FString&ImportError, TArray<FVector>&Section, int32 index);
	
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

};
