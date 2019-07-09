// Fill out your copyright notice in the Description page of Project Settings.
#include "RndrCharacter.h"
#include "CollisionQueryParams.h"
//#include "EditableMesh/Public/EditableMesh.h"

ARndrCharacter::ARndrCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool ARndrCharacter::MaTrace(AActor*actorToIgnore, const FVector&start, const FVector&end, 
	FHitResult&hitOut, ECollisionChannel collisoinChannel/*=ECC_Pawn*/)
{
	FCollisionQueryParams TraceParams(FName(TEXT("VictoreCore Trace")), true, actorToIgnore);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnFaceIndex = true;
	TraceParams.AddIgnoredActor(actorToIgnore);
	hitOut = FHitResult(ForceInit);
	this->GetWorld()->LineTraceSingleByChannel(hitOut, start, end, collisoinChannel, TraceParams);
	return (hitOut.GetActor()!=NULL);
}

void ARndrCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARndrCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARndrCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

