//#remober struct?
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RndrNode.generated.h"

USTRUCT(BlueprintType)
struct FNodeInfoMat
{
	GENERATED_BODY()
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		FString MatType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		TArray<FString>PropertyName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		TArray<FString>PropertyValue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		TArray<FString>PropertyNameBRDF;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		TArray<FString>PropertyValueBRDF;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		TArray<FString>uiGuides;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		TArray<FString>defaultVal;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		TArray<FString>type;

	FNodeInfoMat()
	{
	}
};
USTRUCT(BlueprintType)
struct FNodeInfo
{
	GENERATED_BODY()
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		FString NodeName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		FString NodeMaterialName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		FVector NodeTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		FNodeInfoMat NodeMaterial;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		TArray<FString> NodeProperties;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "vray")
		TArray<FString> NodePropertiesValues;
	FNodeInfo()
	{
	}
};

UCLASS()
class RNDR_API ARndrNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARndrNode();


	UPROPERTY(BlueprintReadWrite,EditAnywhere,  Category="vray")
		 FNodeInfo NodeInfo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
