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
	ELightRectangle UMETA(DisplayName = "LightRectangle"),
	ELightSphere UMETA(DisplayName = "LightSphere"),
	ECamera UMETA(DisplayName = "Camera"),
	EAll UMETA(DisplayName = "All")
};

UCLASS()
class RNDR_API AVrayInterface : public AActor
{
	GENERATED_BODY()
public:	
	AVrayInterface();
	VRay::VRayRenderer renderer;
	UFUNCTION(BlueprintCallable, Category = "vray")
		void LoadScene(FString path);
	UFUNCTION(BlueprintCallable, Category = "vray")
		bool DeletePluginCpp(FString PluginName);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void CreatePluginCpp(FString&PluginNameOut, EVrayPluginType PluginType);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void Render(int option);
	UFUNCTION(BlueprintPure, Category = "vray")
		void GetVrayNodeNames(TArray<FString>&PluginType, TArray<FString>&PluginName);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void SetVrayPluginParameter(bool&ParamSetSuccessfully, EVrayPluginType PluginType, TArray<FVector>transformIn, FString nameIn, FLinearColor colorIn, int32 intIn, TArray<float>floatArrayIn, FString ParameterName);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void GetVrayPluginParameter(TArray<FString>&propertyNamesOut, TArray<FString>&PropertyValuesOut, TArray<FString>&ParamTypeOut, EVrayPluginType PluginType, TArray<FVector>&transformOut, FString nameIn,FLinearColor&colorOut, int32&intOut, TArray<float>&floatArrayOut, bool&paramFound, FString ParameterName,FString&ParameterValue);
 	UFUNCTION(BlueprintCallable, Category = "vray")
 		void CreateGeomStaticMesh(TArray<FVector>UnrealVertices, TArray<FVector>UnrealNormals,TArray<int32>UnrealFaces, TArray<int32>UnrealFaceNormals, FString NodeName);

protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
};
