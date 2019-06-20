// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "assimpComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RNDR_API UassimpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UassimpComponent();
	UFUNCTION(BlueprintCallable, Category = "Assimp")
		bool NewLoadMesh(FString FilePath, TArray<FVector>&vertices, 
		TArray<FVector>&normals, TArray<int32>&faces, TArray<int32>&faceNormals);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
