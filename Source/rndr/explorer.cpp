#include "explorer.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"



Aexplorer::Aexplorer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void Aexplorer::BeginPlay()
{
	Super::BeginPlay();
	
}

void Aexplorer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
 

void Aexplorer::OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames)
{
	if (GEngine)
	{
		if (GEngine->GameViewport)
		{
			void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (DesktopPlatform)
			{
				uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
				DesktopPlatform->OpenFileDialog(ParentWindowHandle, DialogTitle, DefaultPath, FString(""), FileTypes, SelectionFlag, OutFileNames);
			}
		}
	}
}

void Aexplorer::PrintData(const FString& File)
{
	//Parse the data into a string array
	TArray<FString> LoadedText;
	FFileHelper::LoadFileToStringArray(LoadedText, *File);
	//Print the contents
	for (int32 i = 0; i < LoadedText.Num(); i++)
	{
		GLog->Log(LoadedText[i]);
	}
}