//Copyright 2019 Jose Jacques. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "explorer.generated.h"

UCLASS()
class RNDR_API Aexplorer : public AActor
{
	GENERATED_BODY()
	
public:	
	Aexplorer();
	UFUNCTION(BlueprintCallable, Category = "FilePicker")
		void OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames);
	UFUNCTION(BlueprintCallable, Category = "FilePicker")
		void PrintData(const FString& File);
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

};
