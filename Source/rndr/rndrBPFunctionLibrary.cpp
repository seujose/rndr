#include "rndrBPFunctionLibrary.h"
#include "VrayInterface.h"
#include "Engine/Texture2D.h"

void UrndrBPFunctionLibrary::getPixel(UTexture2D*SourceTexture, UTexture2D*&DestinationTexture, int32 pixelX, int32 pixelY, FColor&pixelColorOut)
{ 
	uint8* SourceData = (uint8 *)malloc(256 * 256 * 4);
	UTexture2D*newTexture = UTexture2D::CreateTransient(256, 256, PF_R8G8B8A8);
	uint8* DestinationData = (uint8 *)malloc(256 * 256 * 4);
	//AVrayInterface vrayInterface;
	//vrayInterface.getPixelDataaa(*SourceData);
	

	FTexture2DMipMap*mip = &newTexture->PlatformData->Mips[0];

	mip->SizeX = 256;
	mip->SizeY = 256;
	mip->BulkData.Lock(LOCK_READ_WRITE);

	FMemory::Memcpy(DestinationData, SourceData, sizeof(uint8) * 256 * 256 * 4);
	mip->BulkData.Unlock();
	newTexture->AddToRoot();
	newTexture->UpdateResource();
	DestinationTexture = newTexture;
	free(SourceData);
	SourceData = NULL;
	

	

	/*size_t temp=12345;

	uint8* temmmp=nullptr; 
	temmmp= (uint8*)temp;
	FTexture2DMipMap*MipMapFromSourceTexture = &SourceTexture->PlatformData->Mips[0];

	FUntypedBulkData*rawDataFromMip = &MipMapFromSourceTexture->BulkData;

	FColor*castedFColorFromRawData = static_cast<FColor*>(rawDataFromMip->Lock(LOCK_READ_ONLY));


	uint32 textureWidth = MipMapFromSourceTexture->SizeX, textureHeight = MipMapFromSourceTexture->SizeY;
	FColor pixelColor;
	pixelColor = castedFColorFromRawData[pixelY*textureWidth + pixelX];
	pixelColorOut = pixelColor;

	SourceTexture->PlatformData->Mips[0].BulkData.Unlock();*/
}
 