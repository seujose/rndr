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
void AVrayInterface::SetVrayPluginParameter(FString ParameterName, TArray<float> ParameterValue, FString NodeName)
{
	vector<string>propertyNames;
	FString fstringTemp;
	string stringTemp = TCHAR_TO_UTF8(*NodeName);
	Node node  = renderer.getPlugin<Node>(stringTemp);

	//material
	Plugin GenericMat = renderer.getPlugin(node.get_material().getName());
	MtlSingleBRDF GenericMtlSingleBrdf = plugin_cast<MtlSingleBRDF>(GenericMat);
	BRDFVRayMtl  GenericBRDFVRayMtl = plugin_cast<BRDFVRayMtl>(GenericMtlSingleBrdf.get_brdf());
	GenericBRDFVRayMtl.setValue(TCHAR_TO_UTF8(*ParameterName), ParameterValue[0]);
}

void AVrayInterface::GetVrayPluginParameter(FString ParameterName, FString&ParameterValue, FString NodeName, EPluginType PluginType, FString&ParameterType)
{ 
	FString fstringTemp;
	string stringTemp;
	
	switch (PluginType)
	{
	case EPluginType::ENode:
	{
		Plugin plugin = renderer.getPlugin(TCHAR_TO_UTF8(*NodeName));
		stringTemp=plugin.getValueAsString(TCHAR_TO_UTF8(*ParameterName));
		ParameterValue = stringTemp.c_str();
		stringTemp=plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getStringType();
		ParameterType = stringTemp.c_str();
	}
		break;

	case EPluginType::EBRDF:
	{
		Plugin plugin = renderer.getPlugin(TCHAR_TO_UTF8(*NodeName));
		Node node = plugin_cast<Node>(plugin);
		MtlSingleBRDF mtl = plugin_cast<MtlSingleBRDF>(node.get_material());
		BRDFVRayMtl BRDF = plugin_cast<BRDFVRayMtl>(mtl.get_brdf());
		stringTemp=BRDF.getValueAsString(TCHAR_TO_UTF8(*ParameterName));
		ParameterValue = stringTemp.c_str();
	}
		break;

	case EPluginType::ELight:
	{

	}
		break;

	case EPluginType::EGeneric:
	{
		Plugin plugin = renderer.getPlugin(TCHAR_TO_UTF8(*NodeName));
		stringTemp = plugin.getValueAsString(TCHAR_TO_UTF8(*ParameterName));
		ParameterValue = stringTemp.c_str();
	}
		break;
	}
}

void AVrayInterface::LoadScene()
{
	renderer.load("C:\\Users\\master\\Documents\\3ds Max 2020\\scenes\\cenaBase.vrscene");
}

TArray<FString> AVrayInterface::GetVrayNodeNames()
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

void AVrayInterface::GetNodeZeroParams()
{
	string stringTemp;
	FString fstringTemp;
	vector<string>propertyNames;
	Node node = renderer.getPlugins<Node>()[0];
	PluginMeta pluginMeta = renderer.getPluginMeta(node.getType());
	propertyNames = pluginMeta.getPropertyNames();
	for (size_t i = 0; i < propertyNames.size(); i++)
	{
		fstringTemp = propertyNames[i].c_str();
		UE_LOG(LogTemp, Warning, TEXT("node zero params (%s)"), *fstringTemp);
	}
}
