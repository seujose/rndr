#include "rndrBPFunctionLibrary.h"
#include "VrayInterface.h"
#include "Engine/Texture2D.h"
void UrndrBPFunctionLibrary::getPixel(UTexture2D*SourceTexture, UTexture2D*&DestinationTexture, int32 pixelX, int32 pixelY, FColor&pixelColorOut)
{ 
	TArray<FColor>colorArray;
	for (size_t i = 0; i < 256 * 256; i++)
	{
		FColor tempColor;
		tempColor.MakeRandomColor();
		colorArray.Add(tempColor);
	}
	
	UTexture2D*newTexture = UTexture2D::CreateTransient(256, 256);
	newTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	newTexture->SRGB = 0;
	newTexture->AddToRoot();

	uint8* SourceData = (uint8 *)malloc(256 * 256 * 4);
	uint8* DestinationData = (uint8 *)malloc(256 * 256 * 4);
	//AVrayInterface vrayInterface;
	//vrayInterface.getPixelDataaa(*SourceData);
	
	FTexture2DMipMap&mip = newTexture->PlatformData->Mips[0];

	mip.BulkData.Lock(LOCK_READ_WRITE);
	//mip->BulkData.Lock(LOCK_READ_WRITE);

	auto stride = (int32)(sizeof(uint8) * 4); // for r, g, b, a
	FMemory::Memcpy(DestinationData, colorArray.GetData(), 256*256*stride);
	mip.BulkData.Unlock();
	newTexture->UpdateResource();
	DestinationTexture = newTexture;
	free(SourceData);
	SourceData = NULL;
}


void UrndrBPFunctionLibrary::newGetPixel(UTexture2D*SourceTexture, UTexture2D*&DestinationTexture, int32 pixelX, int32 pixelY, FColor&pixelColorOut)
{
	int32 SizeX = 256;
	int32 SizeY = 256;
	UTexture2D*textureGraph = UTexture2D::CreateTransient(SizeX, SizeY);
	textureGraph->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	textureGraph->SRGB = 0;

	EPixelFormat pixelFormat = textureGraph->PlatformData->PixelFormat;
	FTexture2DMipMap* graphMipMap = &textureGraph->PlatformData->Mips[0];

	FByteBulkData* rawImageData = &graphMipMap->BulkData;
	uint8 * pImageData = (uint8 *)rawImageData->Lock(LOCK_READ_WRITE);

	uint32 TextureWidth = SizeX, TextureHeight = SizeY;
	FColor PixelColor;

	PixelColor.R = 255;
	PixelColor.G = 0;
	PixelColor.B = 0;
	PixelColor.A = 255;

	uint32 DrawLineX = 256;
	uint32 DrawLineY = 256;

	for (uint32 j = 0; j < DrawLineY; j++) {
		for (uint32 i = 0; i < DrawLineX; i++) {
			uint8 *pPixelAddress = &pImageData[((j * (TextureWidth)) + (i)) * 4];
			pPixelAddress[0] = PixelColor.B;
			pPixelAddress[1] = PixelColor.G;
			pPixelAddress[2] = PixelColor.R;
			pPixelAddress[3] = PixelColor.A;
		}
	}
	textureGraph->PlatformData->Mips[0].BulkData.Unlock();
	textureGraph->UpdateResource();
	DestinationTexture = textureGraph;
}
