// Copyright 2019 Jose Jacques. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "rndrRawPixel.generated.h"

UCLASS()
class RNDR_API ArndrRawPixel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ArndrRawPixel();
	UFUNCTION(BlueprintCallable, Category = "rawPixel")
		void getPixel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
