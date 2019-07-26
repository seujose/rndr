#include "rndrBPFunctionLibrary.h"
#include "Engine/Texture2D.h"

 


void UrndrBPFunctionLibrary::getPixel(UTexture2D*SourceTexture, UTexture2D*&finalTexture)
{ 
	FTexture2DMipMap*sourceMip = &SourceTexture->PlatformData->Mips[0];
	FByteBulkData*sourceData = &sourceMip->BulkData;

	UTexture2D*transientTexture = UTexture2D::CreateTransient(256, 256);

	FTexture2DMipMap*destinationMip = &transientTexture->PlatformData->Mips[0];
	FByteBulkData*destinationData = &destinationMip->BulkData;
	destinationData->Lock(LOCK_READ_ONLY);

	auto stride = (int32)(sizeof(uint8) * 4);
	FMemory::Memcpy(destinationData, sourceData, 100 * 100*stride);
	destinationData->Unlock();
	finalTexture = transientTexture;
}
