// Copyright 2019 Jose Jacques. All Rights Reserved.

#include "DrawingCanvas.h"

UDrawingCanvas::UDrawingCanvas()
{
}

UDrawingCanvas::~UDrawingCanvas()
{
}


void UDrawingCanvas::InitializeCanvas(const int32 pixelsH, const int32 pixelsV)
{
	//dynamic texture initialization
	canvasWidth = pixelsH;
	canvasHeight = pixelsV;

	dynamicCanvas = UTexture2D::CreateTransient(canvasWidth, canvasHeight);
#if WITH_EDITORONLY_DATA
	dynamicCanvas->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
#endif
	dynamicCanvas->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	dynamicCanvas->SRGB = 1;
	dynamicCanvas->AddToRoot();
	dynamicCanvas->Filter = TextureFilter::TF_Nearest;
	dynamicCanvas->UpdateResource();

	echoUpdateTextureRegion = std::unique_ptr<FUpdateTextureRegion2D>(new FUpdateTextureRegion2D(0, 0, 0, 0, canvasWidth, canvasHeight));


	// buffers initialization
	bytesPerPixel = 4; // r g b a
	bufferPitch = canvasWidth * bytesPerPixel;
	bufferSize = canvasWidth * canvasHeight * bytesPerPixel;
	canvasPixelData = std::unique_ptr<uint8[]>(new uint8[bufferSize]);

	ClearCanvas();
}

void UDrawingCanvas::InitializeDrawingTools(const int32 brushRadius)
{
	radius = brushRadius;
	brushBufferSize = radius * radius * 4 * bytesPerPixel; //2r*2r * bpp
	canvasBrushMask = std::unique_ptr<uint8[]>(new uint8[brushBufferSize]);
	uint8* canvasBrushPixelPtr = canvasBrushMask.get();
	for (int px = -radius; px < radius; ++px)
	{
		for (int py = -radius; py < radius; ++py)
		{
			int32 tx = px + radius;
			int32 ty = py + radius;
			canvasBrushPixelPtr = canvasBrushMask.get() + (tx + +ty * 2 * radius) * bytesPerPixel;
			if (px*px + py * py < radius*radius)
			{
				setPixelColor(canvasBrushPixelPtr, 0, 0, 0, 255); //black alpha 255 - bgra
			}
			else
			{
				setPixelColor(canvasBrushPixelPtr, 0, 0, 0, 0); // alpha 0
			}
		}
	}
}

void UDrawingCanvas::DrawDot(const int32 pixelCoordX, const int32 pixelCoordY)
{
	uint8* canvasPixelPtr = canvasPixelData.get();
	const uint8* canvasBrushPixelPtr = canvasBrushMask.get();
	for (int px = -radius; px < radius; ++px)
	{
		for (int py = -radius; py < radius; ++py)
		{
			int32 tbx = px + radius;
			int32 tby = py + radius;

			canvasBrushPixelPtr = canvasBrushMask.get() + (tbx + tby * 2 * radius) * bytesPerPixel;
			if (*(canvasBrushPixelPtr + 3) == 255) // check the alpha value of the pixel of the brush mask
			{
				int32 tx = pixelCoordX + px;
				int32 ty = pixelCoordY + py;
				if (tx >= 0 && tx < canvasWidth && ty >= 0 && ty < canvasHeight)
				{
					canvasPixelPtr = canvasPixelData.get() + (tx + ty * canvasWidth) * bytesPerPixel;
					setPixelColor(canvasPixelPtr, *(canvasBrushPixelPtr + 2), *(canvasBrushPixelPtr + 1), *(canvasBrushPixelPtr), *(canvasBrushPixelPtr + 3));
				}
			}
		}
	}

	UpdateCanvas();
}

void UDrawingCanvas::ClearCanvas()
{
	uint8* canvasPixelPtr = canvasPixelData.get();
	for (int i = 0; i < canvasWidth * canvasHeight; ++i)
	{
		setPixelColor(canvasPixelPtr, 255, 255, 255, 255); //white
		canvasPixelPtr += bytesPerPixel;
	}
	UpdateCanvas();
}

void UDrawingCanvas::UpdateCanvas()
{
	if (echoUpdateTextureRegion)
	{
		dynamicCanvas->UpdateTextureRegions((int32)0, (uint32)1, echoUpdateTextureRegion.get(), (uint32)bufferPitch, (uint32)bytesPerPixel, canvasPixelData.get());
	}
}

void UDrawingCanvas::setPixelColor(uint8*& pointer, uint8 red, uint8 green, uint8 blue, uint8 alpha)
{
	*pointer = blue; //b
	*(pointer + 1) = green; //g
	*(pointer + 2) = red; //r
	*(pointer + 3) = alpha; //a
}
