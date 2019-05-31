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

void AVrayInterface::GetVrayPluginParameter(TArray<FVector>&transformOut, FString nameIn, FLinearColor&colorOut, int32&intOut, TArray<float>&floatArrayOut, bool&boolean, FString ParameterName, FString&ParameterValue)
{ 

	Plugin plugin = renderer.getPlugin(TCHAR_TO_UTF8(*nameIn));
	Type paramType = plugin.getValue(TCHAR_TO_UTF8(*ParameterName), boolean).getType();
	
	if (boolean)
	{
	switch (paramType)
	{
	case VRay::TYPE_INT:
	{
		intOut = plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getInt();
	}
		break;
	case VRay::TYPE_FLOAT:
	{
		floatArrayOut[0]= plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getFloat();
	}
		break;
	case VRay::TYPE_DOUBLE:
		break;
	case VRay::TYPE_BOOL:
		break;
	case VRay::TYPE_VECTOR:
		break;
	case VRay::TYPE_COLOR:
	{
		Color color = plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getColor();
		colorOut.R = color.r;
		colorOut.G = color.g;
		colorOut.B = color.b;
	}
		break;
	case VRay::TYPE_ACOLOR:
	{
		AColor acolor =  plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getAColor();
		colorOut.R = acolor.color.r;
		colorOut.G = acolor.color.g;
		colorOut.B = acolor.color.b;
		colorOut.A = acolor.alpha;
	}
		break;
	case VRay::TYPE_MATRIX:
		break;
	case VRay::TYPE_TRANSFORM:
	{
		VRay::Transform transform = plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getTransform();
		FVector temp;
		transformOut.Init(temp, 4);

		for (size_t i = 0; i < 2; i++)
		{
			transformOut[i].X=transform.matrix[i].x;
			transformOut[i].Y = transform.matrix[i].y;
			transformOut[i].Z = transform.matrix[i].z;
		}

		transformOut[3].X = transform.offset.x;
		transformOut[3].Y = transform.offset.y;
		transformOut[3].Z = transform.offset.z;
	}
		break;
	case VRay::TYPE_STRING:
		break;
	case VRay::TYPE_PLUGIN:
		break;
	case VRay::TYPE_TEXTURE:
		break;
	case VRay::TYPE_LIST:
		break;
	case VRay::TYPE_TEXTUREFLOAT:
		break;
	case VRay::TYPE_TEXTUREINT:
		break;
	case VRay::TYPE_TEXTUREVECTOR:
		break;
	case VRay::TYPE_TEXTUREMATRIX:
		break;
	case VRay::TYPE_TEXTURETRANSFORM:
		break;
	//case VRay::TYPE_GENERAL_LIST:
		//break;
	case VRay::TYPE_INT_LIST:
		break;
	case VRay::TYPE_FLOAT_LIST:
		break;
	case VRay::TYPE_BOOL_LIST:
		break;
	case VRay::TYPE_VECTOR_LIST:
		break;
	case VRay::TYPE_COLOR_LIST:
		break;
	case VRay::TYPE_STRING_LIST:
		break;
	case VRay::TYPE_PLUGIN_LIST:
		break;
	case VRay::TYPE_TEXTURE_LIST:
		break;
	case VRay::TYPE_TEXTUREFLOAT_LIST:
		break;
	case VRay::TYPE_TEXTUREMATRIX_LIST:
		break;
	case VRay::TYPE_TEXTURETRANSFORM_LIST:
		break;
	case VRay::TYPE_OUTPUTTEXTURE:
		break;
	case VRay::TYPE_OUTPUTTEXTUREFLOAT:
		break;
	case VRay::TYPE_OUTPUTTEXTUREINT:
		break;
	case VRay::TYPE_OUTPUTTEXTUREVECTOR:
		break;
	case VRay::TYPE_OUTPUTTEXTUREMATRIX:
		break;
	case VRay::TYPE_OUTPUTTEXTURETRANSFORM:
		break;
	case VRay::TYPE_UNSPECIFIED:
		break;
	case VRay::TYPE_ERROR:
		break;
	default:
		break;
	}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("param not found"));
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

void AVrayInterface::GetPluginPropertyNamesValuesTypes(EVrayPluginType PluginType)
{
	string stringTemp;
	FString fstringTemp;
	vector<string>propertyNames;
	Node node = renderer.getPlugins<Node>()[0];
	Plugin plugin = renderer.getPlugin(node.get_material().getName());
	MtlSingleBRDF mtlSingleBRDF = plugin_cast<MtlSingleBRDF>(plugin);

	switch (PluginType)
	{
	case EVrayPluginType::ENode:
	{
		PluginMeta pluginMeta = renderer.getPluginMeta(node.getType());
		propertyNames = pluginMeta.getPropertyNames();
		for (size_t i = 0; i < propertyNames.size(); i++)
		{
			PropertyMeta propertyMeta = pluginMeta.getPropertyMeta(propertyNames[i]);

			fstringTemp = propertyNames[i].c_str();
			UE_LOG(LogTemp, Warning, TEXT("name (%s)"), *fstringTemp);

			stringTemp = node.getValue(propertyNames[i]).getStringType();
			fstringTemp = stringTemp.c_str();
			UE_LOG(LogTemp, Warning, TEXT("type (%s)"), *fstringTemp);

			stringTemp = propertyMeta.getUIGuides();
			fstringTemp = stringTemp.c_str();
			UE_LOG(LogTemp, Warning, TEXT("UI (%s)"), *fstringTemp);
		}
	}
		break;
	case EVrayPluginType::EMaterial:
	{
		PluginMeta pluginMeta = renderer.getPluginMeta(mtlSingleBRDF.getType());
		propertyNames = pluginMeta.getPropertyNames();
		for (size_t i = 0; i < propertyNames.size(); i++)
		{
			PropertyMeta propertyMeta = pluginMeta.getPropertyMeta(propertyNames[i]);

			fstringTemp = propertyNames[i].c_str();
			UE_LOG(LogTemp, Warning, TEXT("name (%s)"), *fstringTemp);

			stringTemp = mtlSingleBRDF.getValue(propertyNames[i]).getStringType();
			fstringTemp = stringTemp.c_str();
			UE_LOG(LogTemp, Warning, TEXT("type (%s)"), *fstringTemp);

			stringTemp = propertyMeta.getUIGuides();
			fstringTemp = stringTemp.c_str();
			UE_LOG(LogTemp, Warning, TEXT("UI (%s)"), *fstringTemp);
		}
	}
		break;
	case EVrayPluginType::EBRDF:
	{
		PluginMeta pluginMeta = renderer.getPluginMeta(mtlSingleBRDF.get_brdf().getType());
		propertyNames = pluginMeta.getPropertyNames();
		for (size_t i = 0; i < propertyNames.size(); i++)
		{
			PropertyMeta propertyMeta = pluginMeta.getPropertyMeta(propertyNames[i]);

			fstringTemp = propertyNames[i].c_str();
			UE_LOG(LogTemp, Warning, TEXT("name (%s)"), *fstringTemp);

			stringTemp = mtlSingleBRDF.get_brdf().getValue(propertyNames[i]).getStringType();
			fstringTemp = stringTemp.c_str();
			UE_LOG(LogTemp, Warning, TEXT("type (%s)"), *fstringTemp);

			stringTemp = propertyMeta.getUIGuides();
			fstringTemp = stringTemp.c_str();
			UE_LOG(LogTemp, Warning, TEXT("UI (%s)"), *fstringTemp);
		}
	}
		break;
	case EVrayPluginType::EGeneric:
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPluginPropertyNamesValuesTypes generic switch"));
	}
		break;
	default:
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPluginPropertyNamesValuesTypes default switch"));
	}
		break;
	}
}
