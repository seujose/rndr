#pragma once
#include "CoreMinimal.h"
#include "RndrNode.h"
#include "GameFramework/Actor.h"
#include "vraysdk.hpp"
#include "vrayplugins.hpp"
#include "VrayInterface.generated.h"

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
		TArray<FString> GetPluginsInfo();

	UFUNCTION(BlueprintCallable, Category = "vray")
		void RefreshNodeInfo(ARndrNode*RndrNode);

	VRay::VRayRenderer renderer;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
