#pragma once
#include "CoreMinimal.h"
#include "RndrNode.h"
#include "GameFramework/Actor.h"
#include "vraysdk.hpp"
#include "vrayplugins.hpp"
#include "VrayInterface.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EVrayPluginType : uint8
{
	ENode UMETA(DisplayName = "Node"),
	EMaterial UMETA(DisplayName = "Material"),
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

	VRay::VRayRenderer renderer;


	UFUNCTION(BlueprintCallable, Category = "vray")
		void LoadScene();

	UFUNCTION(BlueprintCallable, Category = "vray")
		void Render(int option);

	UFUNCTION(BlueprintPure, Category = "vray")
		TArray<FString> GetVrayNodeNames();

	UFUNCTION(BlueprintCallable, Category = "vray")
		void GetPluginPropertyNamesValuesTypes(EVrayPluginType PluginType);

	UFUNCTION(BlueprintCallable, Category = "vray")
		void SetVrayPluginParameter(bool&ParamSetSuccessfully, EVrayPluginType PluginType, TArray<FVector>transformIn, FString nameIn, FLinearColor colorIn, int32 intIn, TArray<float>floatArrayIn, bool&boolean, FString ParameterName, FString ParameterValue);

	UFUNCTION(BlueprintCallable, Category = "vray")
		void GetVrayPluginParameter(EVrayPluginType PluginType, TArray<FVector>&transformOut, FString nameIn, FLinearColor&colorOut, int32&intOut, TArray<float>&floatArrayOut, bool&boolean, FString ParameterName, FString&ParameterValue);
	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
