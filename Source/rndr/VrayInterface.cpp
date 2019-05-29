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
	
	/************************************************************************/
	/* list properties                                                      */
	/************************************************************************/
	PluginMeta pluginMeta = renderer.getPluginMeta(GenericBRDFVRayMtl.getType());
	propertyNames = pluginMeta.getPropertyNames();
	for (size_t i = 0; i < propertyNames.size(); i++)
	{
		//property name
		fstringTemp = UTF8_TO_TCHAR(propertyNames[i].c_str());
		//RndrNode->NodeInfo.NodeMaterial.PropertyNameBRDF.Push(fstringTemp);
		UE_LOG(LogTemp, Warning, TEXT("property name(%s)"), *fstringTemp);

		//property value
		stringTemp=GenericBRDFVRayMtl.getValueAsString(propertyNames[i]);
		fstringTemp = stringTemp.c_str();
		//RndrNode->NodeInfo.NodeMaterial.PropertyValueBRDF.Push(fstringTemp);
		UE_LOG(LogTemp, Warning, TEXT("property value(%s)"), *fstringTemp);

		//property type
		stringTemp = GenericBRDFVRayMtl.getValue(propertyNames[i]).getStringType();
		fstringTemp = stringTemp.c_str();
		//RndrNode->NodeInfo.NodeMaterial.type.Push(fstringTemp);
		UE_LOG(LogTemp, Warning, TEXT("property type(%s)"), *fstringTemp);
	}
}



void AVrayInterface::GetVrayPluginParameter(FString ParameterName, FString&ParameterValue, FString NodeName)
{
	FString fstringTemp;
	string stringTemp = TCHAR_TO_UTF8(*NodeName);
	Node node = renderer.getPlugin<Node>(stringTemp);

	//material
	Plugin GenericMat = renderer.getPlugin(node.get_material().getName());
	MtlSingleBRDF GenericMtlSingleBrdf = plugin_cast<MtlSingleBRDF>(GenericMat);
	BRDFVRayMtl  GenericBRDFVRayMtl = plugin_cast<BRDFVRayMtl>(GenericMtlSingleBrdf.get_brdf());
	stringTemp=GenericBRDFVRayMtl.getValueAsString(TCHAR_TO_UTF8(*ParameterName));
	ParameterValue=stringTemp.c_str();
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
