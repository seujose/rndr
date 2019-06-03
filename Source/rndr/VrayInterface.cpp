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
void AVrayInterface::CreatePlugin(FString PluginName, EVrayPluginType PluginType)
{
	switch (PluginType)
	{
	case EVrayPluginType::ENode:
	{
		Node node = renderer.newPlugin<Node>(TCHAR_TO_UTF8(*PluginName));
	}
	break;
	case EVrayPluginType::EMaterial:
	{
		MtlSingleBRDF mtlSingleBRDF = renderer.newPlugin<MtlSingleBRDF>(TCHAR_TO_UTF8(*PluginName));
	}
		break;
	case EVrayPluginType::EBRDF:
	{
		BRDFVRayMtl brdf  = renderer.newPlugin<BRDFVRayMtl>(TCHAR_TO_UTF8(*PluginName));
	}
		break;
	case EVrayPluginType::ELight:
	{
		LightRectangle light  = renderer.newPlugin<LightRectangle>(TCHAR_TO_UTF8(*PluginName));
	}
		break;
	case EVrayPluginType::ECamera:
	{
		CameraPhysical camera = renderer.newPlugin<CameraPhysical>(TCHAR_TO_UTF8(*PluginName));
	}
		break;
	case EVrayPluginType::EGeneric:
		break;
	default:
		break;
	}
}
bool AVrayInterface::DeletePlugin(FString PluginName)
{
	Plugin plugin = renderer.getPlugin(TCHAR_TO_UTF8(*PluginName));
	string tempString = plugin.getType();
	if (tempString.compare("Node")==0)
	{
		Node node = renderer.getPlugin<Node>(TCHAR_TO_UTF8(*PluginName));
		renderer.deletePlugin(node);
		return true;
	}
	return false;
}

void AVrayInterface::SetVrayPluginParameter(bool&ParamSetSuccessfully, EVrayPluginType PluginType, TArray<FVector>transformIn, FString nameIn, FLinearColor colorIn, int32 intIn, TArray<float>floatArrayIn, bool&boolean, FString ParameterName, FString ParameterValue)
{
	ParamSetSuccessfully = false;
	Plugin plugin;
	switch (PluginType)
	{
	case EVrayPluginType::ENode:
	{
		Node node = renderer.getPlugin<Node>(TCHAR_TO_UTF8(*nameIn));
		plugin = node;
	}
		break;
	case EVrayPluginType::EMaterial:
	{
		Node node = renderer.getPlugin<Node>(TCHAR_TO_UTF8(*nameIn));
		MtlSingleBRDF mtlSingleBRDF = renderer.getPlugin<MtlSingleBRDF>(node.get_material().getName());
		plugin = mtlSingleBRDF;
	}
		break;
	case EVrayPluginType::EBRDF:
	{
		Node node = renderer.getPlugin<Node>(TCHAR_TO_UTF8(*nameIn));
		MtlSingleBRDF mtlSingleBRDF = renderer.getPlugin<MtlSingleBRDF>(node.get_material().getName());
		BRDFVRayMtl bRDFVRayMtl = plugin_cast<BRDFVRayMtl>(mtlSingleBRDF.get_brdf());
		plugin = bRDFVRayMtl;
	}
		break;
	case EVrayPluginType::ELight:
		break;
	case EVrayPluginType::ECamera:
	{
		RenderView renderView = renderer.getInstanceOrCreate<RenderView>();
		plugin = renderView;
	}
		break;
	case EVrayPluginType::EGeneric:
		break;
	default:
		break;
	}

	Type paramType = plugin.getValue(TCHAR_TO_UTF8(*ParameterName), boolean).getType();


	if (boolean)
	{
		switch (paramType)
		{
		case VRay::TYPE_INT:
		{
			intIn = plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getInt();
		}
		break;
		case VRay::TYPE_FLOAT:
		{
			ParamSetSuccessfully = (plugin.setValue(TCHAR_TO_UTF8(*ParameterName), floatArrayIn[0]));
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
			colorIn.R = color.r;
			colorIn.G = color.g;
			colorIn.B = color.b;
		}
		break;
		case VRay::TYPE_ACOLOR:
		{
			AColor acolor = plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getAColor();
			colorIn.R = acolor.color.r;
			colorIn.G = acolor.color.g;
			colorIn.B = acolor.color.b;
			colorIn.A = acolor.alpha;
		}
		break;
		case VRay::TYPE_MATRIX:
			break;
		case VRay::TYPE_TRANSFORM:
		{
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
		UE_LOG(LogTemp, Warning, TEXT("%s not found"), *ParameterName);
	}
	 
}
void AVrayInterface::GetVrayPluginParameter(EVrayPluginType PluginType, TArray<FVector>&transformOut, FString nameIn, FLinearColor&colorOut, int32&intOut, TArray<float>&floatArrayOut, bool&paramFound, FString ParameterName, FString&ParameterValue)
{ 
	string tempString;
	FString tempFString;
	floatArrayOut.Init(0.0, 4);
	Plugin plugin;
	switch (PluginType)
	{
	case EVrayPluginType::ENode:
	{
		Node node = renderer.getPlugin<Node>(TCHAR_TO_UTF8(*nameIn));
		plugin = node;
	}
		break;
	case EVrayPluginType::EMaterial:
	{
		Node node = renderer.getPlugin<Node>(TCHAR_TO_UTF8(*nameIn));
		MtlSingleBRDF mtlSingleBRDF = renderer.getPlugin<MtlSingleBRDF>(node.get_material().getName());
		plugin = mtlSingleBRDF;
	}
		break;
	case EVrayPluginType::EBRDF:
	{
		Node node = renderer.getPlugin<Node>(TCHAR_TO_UTF8(*nameIn));
		MtlSingleBRDF mtlSingleBRDF = renderer.getPlugin<MtlSingleBRDF>(node.get_material().getName());
		BRDFVRayMtl bRDFVRayMtl = plugin_cast<BRDFVRayMtl>(mtlSingleBRDF.get_brdf());
		plugin = bRDFVRayMtl;
	}
		break;
	case EVrayPluginType::ELight:
		break;
	case EVrayPluginType::EGeneric:
		break;
	default:
		break;
	}

	Type paramType = plugin.getValue(TCHAR_TO_UTF8(*ParameterName), paramFound).getType();
	
	if (paramFound)
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
		tempString = plugin.getType();
		tempString = tempString.c_str();
		UE_LOG(LogTemp, Warning, TEXT("%s not found-type is %s"), *ParameterName, *tempFString);
	}
}

void AVrayInterface::LoadScene()
{
	renderer.load("C:\\Users\\master\\Documents\\3ds Max 2020\\scenes\\cenaBase.vrscene");
}

void AVrayInterface::Render(int option)
{
	switch (option)
	{
		case 0:
		{
			SettingsGI gi = renderer.getInstanceOrCreate<SettingsGI>();
			gi.set_on(true);
			gi.set_primary_engine(2);
			gi.set_secondary_engine(3);
			renderer.startSync();
		}
		break;
		case 1:
		{

		}
		break;
	}
}

void AVrayInterface::GetVrayNodeNames(TArray<FString>&PluginType, TArray<FString>&PluginName)
{
	string tempString;
	for (const Plugin& plugin : renderer.getPlugins())
	{
		tempString = plugin.getType();
		PluginType.Push(tempString.c_str());
		
		tempString = plugin.getName();
		PluginName.Push(tempString.c_str());
	}
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
	case EVrayPluginType::ECamera:
	{
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
