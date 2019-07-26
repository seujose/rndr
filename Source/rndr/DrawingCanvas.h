// Copyright 2019 Jose Jacques. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include <memory>
#include "Engine/Texture2D.h"
#include "UObject/NoExportTypes.h"
#include "Object.h"
#include "DrawingCanvas.generated.h"
UCLASS(Blueprintable, BlueprintType)
class RNDR_API UDrawingCanvas : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables)
		UTexture2D* dynamicCanvas;
	UFUNCTION(BlueprintCallable, Category = DrawingTools)
		void InitializeCanvas(const int32 pixelsH, const int32 pixelsV);
	UFUNCTION(BlueprintCallable, Category = DrawingTools)
		void UpdateCanvas();
	UFUNCTION(BlueprintCallable, Category = DrawingTools)
		void ClearCanvas();
	UFUNCTION(BlueprintCallable, Category = DrawingTools)
		void InitializeDrawingTools(const int32 brushRadius);
	UFUNCTION(BlueprintCallable, Category = DrawingTools)
		void DrawDot(const int32 pixelCoordX, const int32 pixelCoordY);
	UDrawingCanvas();
	~UDrawingCanvas();
private:
	// canvas
	std::unique_ptr<uint8[]> canvasPixelData;
	int canvasWidth;
	int canvasHeight;
	int bytesPerPixel;
	int bufferPitch;
	int bufferSize;

	// draw brush tool
	std::unique_ptr<uint8[]> canvasBrushMask;
	int radius;
	int brushBufferSize;

	std::unique_ptr<FUpdateTextureRegion2D> echoUpdateTextureRegion;
	void setPixelColor(uint8*& pointer, uint8 red, uint8 green, uint8 blue, uint8 alpha);
};


