#include "rndrBPFunctionLibrary.h"
#include "Engine/Texture2D.h"

void UrndrBPFunctionLibrary::getPixel(UTexture2D*SourceTexture, UTexture2D*&DestinationTexture, int32 pixelX, int32 pixelY, FColor&pixelColorOut)
{ 
	FTexture2DMipMap*MipMapFromSourceTexture = &SourceTexture->PlatformData->Mips[0];

	FUntypedBulkData*rawDataFromMip = &MipMapFromSourceTexture->BulkData;

	FColor*castedFColorFromRawData = static_cast<FColor*>(rawDataFromMip->Lock(LOCK_READ_ONLY));

	uint32 textureWidth = MipMapFromSourceTexture->SizeX, textureHeight = MipMapFromSourceTexture->SizeY;
	FColor pixelColor;
	pixelColor = castedFColorFromRawData[pixelY*textureWidth + pixelX];
	pixelColorOut = pixelColor;

	SourceTexture->PlatformData->Mips[0].BulkData.Unlock();
}
