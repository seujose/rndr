#pragma once
#include "CoreMinimal.h"
#include "RndrNode.h"
#include "GameFramework/Actor.h"
#include "vraysdk.hpp"
#include "vrayplugins.hpp"
#include "VrayInterface.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EPluginType : uint8
{
	ENode UMETA(DisplayName = "Node"),
	EBRDF UMETA(DisplayName = "BRDF"),
	ELight UMETA(DisplayName = "Light"), 
	EGeneric UMETA(DisplayName = "Generic")
};



UCLASS()
class RNDR_API AVrayInterface : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVrayInterface();
	UFUNCTION(BlueprintCallable, Category = "vray")
		void LoadScene();

	UFUNCTION(BlueprintCallable, Category = "vray")
		TArray<FString> GetVrayNodeNames();

	UFUNCTION(BlueprintCallable, Category = "vray")
		void GetNodeZeroParams();

	UFUNCTION(BlueprintCallable, Category = "vray")
		void SetVrayPluginParameter(FString ParameterName, TArray<float> ParameterValue, FString NodeName);

	UFUNCTION(BlueprintCallable, Category = "vray")
		void GetVrayPluginParameter(FString ParameterName, FString&ParameterValue, FString NodeName, EPluginType PluginType, FString&ParameterType);

	VRay::VRayRenderer renderer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
