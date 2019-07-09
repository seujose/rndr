//#remober struct?
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RndrNode.generated.h"
class UEditableMesh;
UCLASS()
class RNDR_API ARndrNode : public AActor
{
	GENERATED_BODY()
public:	
	ARndrNode();
/*
	UFUNCTION(BlueprintCallable, Category = "rndr")
		int32 getPolygons(UEditableMesh*theMesh, TArray<FVector>&outtt);*/
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
};
