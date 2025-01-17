// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RndrCharacter.generated.h"
//class UEditableMesh;
UCLASS()
class RNDR_API ARndrCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARndrCharacter();
	UFUNCTION(BlueprintCallable, Category = "rndr")
	bool MaTrace(AActor*actorToIgnore, const FVector&start, const FVector&end, FHitResult&hitOut,
		ECollisionChannel collisoinChannel=ECC_Pawn);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
