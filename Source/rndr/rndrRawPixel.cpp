// Copyright 2019 Jose Jacques. All Rights Reserved.


#include "rndrRawPixel.h"

// Sets default values
ArndrRawPixel::ArndrRawPixel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ArndrRawPixel::getPixel()
{
	int32 widht = 1280;
	int32 height = 720;
	//const TArray<uint8>*UncomprBGRA=
	//UTexture2D*tex = UTexture2D::CreateTransient(widht, height, PF_B8G8R8A8);
	/*if (tex)
	{
		//Lock it
		void* TextureData = tex->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		//Copy pixel data
		FMemory::Memcpy(TextureData, UncomprBGRA->GetData(), UncomprBGRA->Num());
		//Unlock
		tex->PlatformData->Mips[0].BulkData.Unlock();
		//Update texture
		tex->UpdateResource();
	}*/

}

// Called when the game starts or when spawned
void ArndrRawPixel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ArndrRawPixel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

