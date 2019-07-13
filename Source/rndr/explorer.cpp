
#include "explorer.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <string.h>

#include "WindowsApplication.h" 
#include "Windows/WindowsSystemIncludes.h" 
#include "AllowWindowsPlatformTypes.h" 


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
 
void Aexplorer::openFile()
{
}

 