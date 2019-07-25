#include "rndrBPFunctionLibrary.h"
FColor UrndrBPFunctionLibrary::getPixel(UTexture2D*MyTexture2D, TArray<FColor>&outColor)
{
	FTexture2DMipMap* MyMipMap = &MyTexture2D->PlatformData->Mips[0];
	FByteBulkData* RawImageData = &MyMipMap->BulkData;
	FColor* FormatedImageData = static_cast<FColor*>(RawImageData->Lock(LOCK_READ_ONLY));
	uint8 PixelX = 5, PixelY = 10;
	uint32 TextureWidth = MyMipMap->SizeX, TextureHeight = MyMipMap->SizeY;
	FColor PixelColor;

	if (PixelX >= 0 && PixelX < TextureWidth && PixelY >= 0 && PixelY < TextureHeight)
	{
		PixelColor = FormatedImageData[PixelY * TextureWidth + PixelX];
	}

	return PixelColor;
}
