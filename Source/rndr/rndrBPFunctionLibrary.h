#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "rndrBPFunctionLibrary.generated.h"
UCLASS()
class RNDR_API UrndrBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "rndrFL")
		static void teste(int32&out);

	

};
