#pragma once

#include "CoreMinimal.h"
#include "RndrNode.h"
#include "GameFramework/Actor.h"
#include "vraysdk.hpp"
#include "vrayplugins.hpp"
#include "VrayInterface.generated.h"

UENUM(BlueprintType)
enum class EVrayPluginType : uint8
{
	ENode UMETA(DisplayName = "Node"),
	EMaterial UMETA(DisplayName = "Material"),
	EBRDF UMETA(DisplayName = "BRDF"),
	ELight UMETA(DisplayName = "Light"), 
	ECamera UMETA(DisplayName = "Camera"),
	EGeneric UMETA(DisplayName = "Generic")
};

UCLASS()
class RNDR_API AVrayInterface : public AActor
{
	GENERATED_BODY()
public:	
	AVrayInterface();
	VRay::VRayRenderer renderer;
	UFUNCTION(BlueprintCallable, Category = "vray")
		void LoadScene();
	UFUNCTION(BlueprintCallable, Category = "vray")
		bool DeletePlugin(FString PluginName);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void CreatePlugin(FString PluginName, EVrayPluginType PluginType);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void Render(int option);
	UFUNCTION(BlueprintPure, Category = "vray")
		void GetVrayNodeNames(TArray<FString>&PluginType, TArray<FString>&PluginName);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void GetPluginPropertyNamesValuesTypes(EVrayPluginType PluginType);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void SetVrayPluginParameter(bool&ParamSetSuccessfully, EVrayPluginType PluginType, TArray<FVector>transformIn, FString nameIn, FLinearColor colorIn, int32 intIn, TArray<float>floatArrayIn, bool&boolean, FString ParameterName, FString ParameterValue);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void GetVrayPluginParameter(EVrayPluginType PluginType, TArray<FVector>&transformOut, FString nameIn,
			FLinearColor&colorOut, int32&intOut, TArray<float>&floatArrayOut, bool&paramFound, FString ParameterName,
			FString&ParameterValue);
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
};
