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
	ERenderView UMETA(DisplayName = "RenderView"),
	EPhysicalCamera UMETA(DisplayName = "PhysicalCamera"),
	EGenericPlugin UMETA(DisplayName = "GenericPlugin")
};

UCLASS()
class RNDR_API AVrayInterface : public AActor
{
	GENERATED_BODY()
public:	
	AVrayInterface();

	VRay::VRayRenderer renderer;

	UFUNCTION(BlueprintCallable, Category = "vray")
		void getGeoInfo(FString PluginName, TArray<FVector>&VerticesOut, TArray<FVector>&NormalsOut,
			TArray<int32>&FacesOut, TArray<int32>&facesNormalsOut, TArray<FVector2D>&UVZeroOut, TArray<FVector2D>&UVOneOut, TArray<int32>&mapChannelfacesOut);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void updateView(TArray<FVector>T);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void bakeAnode(FString nodeName, int32 mode);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void LoadScene(FString path);
	UFUNCTION(BlueprintCallable, Category = "vray")
		bool DeletePluginCpp(FString PluginName);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void CreatePluginCpp(FString&PluginNameOut, EVrayPluginType PluginType);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void Render(int renderMode);
	UFUNCTION(BlueprintPure, Category = "vray")
		void GetVrayNodeNames(TArray<FString>&PluginType, TArray<FString>&PluginName);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void SetVrayPluginParameter(bool&ParamSetSuccessfully, EVrayPluginType PluginType, TArray<FVector>transformIn, FString nameIn, FLinearColor colorIn, int32 intIn, TArray<float>floatArrayIn, FString ParameterName, bool resyncRender);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void GetVrayPluginParameter(TArray<FString>&propertyNamesOut, TArray<FString>&PropertyValuesOut, TArray<FString>&ParamTypeOut, EVrayPluginType PluginType, TArray<FVector>&transformOut, FString nameIn,FLinearColor&colorOut, int32&intOut, TArray<float>&floatArrayOut, bool&paramFound, FString ParameterName,FString&ParameterValue);
	UFUNCTION(BlueprintCallable, Category = "vray")
 		void CreateGeomStaticMesh(TArray<FVector2D>UnrealUVWs, TArray<FVector>UnrealVertices, TArray<FVector>UnrealNormals,TArray<int32>UnrealFaces, TArray<int32>UnrealFaceNormals, FString NodeName);
	UFUNCTION(BlueprintCallable, Category = "vray")
		void ApplyBitmap(FString bitMapPath, FString nodeName);

protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
};
