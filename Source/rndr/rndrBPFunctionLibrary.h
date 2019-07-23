#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "rndrBPFunctionLibrary.generated.h"
UCLASS()
class RNDR_API UrndrBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, DisplayName = "IsNAN", Category = "rndr | Math", meta = (ToolTip = "Returns whether the supplied float is not a number."))
		static bool BlueprintIsNAN(const float TestValue) { return FMath::IsNaN(TestValue); }

	UFUNCTION(BlueprintPure, DisplayName = "IsFinite", Category = "rndr | Math", meta = (ToolTip = "Returns whether the supplied float is finite."))
		static bool BlueprintIsFinite(const float TestValue) { return FMath::IsFinite(TestValue); }

	UFUNCTION(BlueprintPure, Category = "rndr | Math", meta = (ToolTip = "Returns the supplied float if it's valid, zero if it isn't."))
		static float ZeroIfInvalid(const float TestValue) { return (FMath::IsNaN(TestValue) || !FMath::IsFinite(TestValue)) ? 0.0f : TestValue; }

	UFUNCTION(BlueprintPure, Category = "rndr | Math", meta = (ToolTip = "Returns whether the supplied transform contains an invalid value (NaN)."))
		static bool TransformContainsNaN(const FTransform& TestTransform) { return TestTransform.ContainsNaN(); }

	UFUNCTION(BlueprintPure, Category = "rndr | Math", meta = (ToolTip = "Returns whether the supplied vector contains an invalid value (NaN)."))
		static bool VectorContainsNaN(const FVector& TestV) { return TestV.ContainsNaN(); }

	UFUNCTION(BlueprintPure, Category = "rndr | Math", meta = (ToolTip = "Returns whether the supplied rotator contains an invalid value (NaN)."))
		static bool RotatorContainsNaN(const FRotator& TestRot) { return TestRot.ContainsNaN(); }

};
