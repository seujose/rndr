// Fill out your copyright notice in the Description page of Project Settings.


#include "RndrNode.h"

// Sets default values
ARndrNode::ARndrNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARndrNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARndrNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

