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
 