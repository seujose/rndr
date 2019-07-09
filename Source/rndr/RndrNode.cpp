#include "RndrNode.h"
//#include "EditableMesh/Public/EditableMesh.h"
 

ARndrNode::ARndrNode()
{
	PrimaryActorTick.bCanEverTick = true;
}

/*
int32 ARndrNode::getPolygons(UEditableMesh*theMesh,TArray<FVector>&outtt)
{
	return(theMesh->GetPolygonCount());
}*/

void ARndrNode::BeginPlay()
{
	Super::BeginPlay();
}
void ARndrNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

