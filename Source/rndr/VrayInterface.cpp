#include "VrayInterface.h"
#include "vraysdk.hpp"
#include "vrayplugins.hpp"
#include <vector>
#include <string>
#include "VrayInterface.h"

using namespace VRay;
using namespace VRay::Plugins;
using namespace std;
VRay::VRayInit init(true);

AVrayInterface::AVrayInterface()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
void AVrayInterface::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVrayInterface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/************************************************************************/
/* forward declarations                                                 */
/************************************************************************/



/************************************************************************/
/* implementations                                                      */
/************************************************************************/
void AVrayInterface::RefreshNodeInfo(ARndrNode*RndrNode)
{
	//fstringTemp=(stringTemp.c_str());
	//UE_LOG(LogTemp, Warning, TEXT("node transform(%s)"), *fstringTemp);
	vector<string>propertyNames;
	FString fstringTemp;
	string stringTemp = TCHAR_TO_UTF8(*RndrNode->NodeInfo.NodeName);
	Node node  = renderer.getPlugin<Node>(stringTemp);
	//transform
	RndrNode->NodeInfo.NodeTransform.X = node.get_transform().offset.x;
	RndrNode->NodeInfo.NodeTransform.Y = node.get_transform().offset.y;
	RndrNode->NodeInfo.NodeTransform.Z = node.get_transform().offset.z;
	//material
	Plugin genericPlugin = renderer.getPlugin(node.get_material().getName());
	MtlSingleBRDF mtlSingleBRDF = plugin_cast<MtlSingleBRDF>(genericPlugin);
	PluginMeta pluginMeta = renderer.getPluginMeta(mtlSingleBRDF.get_brdf().getType());
	propertyNames = pluginMeta.getPropertyNames();
	for (size_t i = 0; i < propertyNames.size(); i++)
	{
		//property name
		fstringTemp = UTF8_TO_TCHAR(propertyNames[i].c_str());
		RndrNode->NodeInfo.NodeMaterial.PropertyNameBRDF.Push(fstringTemp);
		//property value
		stringTemp=mtlSingleBRDF.get_brdf().getValueAsString(propertyNames[i]);
		fstringTemp = stringTemp.c_str();
		RndrNode->NodeInfo.NodeMaterial.PropertyValueBRDF.Push(fstringTemp);
		//property type
		stringTemp = mtlSingleBRDF.get_brdf().getValue(propertyNames[i]).getStringType();
		fstringTemp = stringTemp.c_str();
		RndrNode->NodeInfo.NodeMaterial.type.Push(fstringTemp);
	}
}

void AVrayInterface::LoadScene()
{
	renderer.load("C:\\Users\\master\\Documents\\3ds Max 2020\\scenes\\cenaBase.vrscene");
}

TArray<FString> AVrayInterface::GetVraySceneInfo()
{
	TArray<FString> out;
	FString temp;
	for (size_t i = 0; i < renderer.getPlugins<Node>().size(); i++)
	{
		temp = renderer.getPlugins<Node>()[i].getName();
		out.Push(temp);
	}
	return out;
}
