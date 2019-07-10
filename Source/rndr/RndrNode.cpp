#include "RndrNode.h"
//#include "EditableMesh/Public/EditableMesh.h"
 

ARndrNode::ARndrNode()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ARndrNode::BeginPlay()
{
	Super::BeginPlay();
}
void ARndrNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

