//#bakParaChaos mostrar pra chaos 
//#criarNode, verificar proporção do mesh
//#sincronizar selecao do menu com viewport
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
//RendererOptions options;

AVrayInterface::AVrayInterface()
{
	PrimaryActorTick.bCanEverTick = true;
	//options.noRenderLicensePreCheck = true;
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
/**************************************\\**********************************/
/* implementations                                                      */
/************************************************************************/

void AVrayInterface::CreatePluginCpp(FString&PluginNameOut, EVrayPluginType PluginType)
{
	switch (PluginType)
	{
	case EVrayPluginType::ENode:
	{
		Node node = renderer.newPlugin<Node>();
		MtlSingleBRDF nodeMat = renderer.newPlugin<MtlSingleBRDF>();
		BRDFVRayMtl nodeMatBrdf = renderer.newPlugin<BRDFVRayMtl>();
		nodeMat.set_brdf(nodeMatBrdf);
		node.set_material(nodeMat);
		node.set_transform(Transform(Matrix(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1)), Vector(0, 0, 0)));
		string temp = node.getName();
		PluginNameOut = temp.c_str();
	}
	break;
	case EVrayPluginType::EMaterial:
	{
		MtlSingleBRDF mtlSingleBRDF = renderer.newPlugin<MtlSingleBRDF>();
		string temp = mtlSingleBRDF.getName();
		PluginNameOut = temp.c_str();
	}
		break;
	case EVrayPluginType::EBRDF:
	{
		BRDFVRayMtl brdf = renderer.newPlugin<BRDFVRayMtl>();
		string temp = brdf.getName();
		PluginNameOut = temp.c_str();
	}
		break;
	case EVrayPluginType::ECamera:
	{
		//somente uma instancia
		CameraPhysical cameraPhysical = renderer.getOrCreatePlugin<CameraPhysical>("cameraPhysical");
		cameraPhysical.set_exposure(true);
		string temp = cameraPhysical.getName();
		PluginNameOut = temp.c_str();

		//somente uma instancia
		RenderView renderView = renderer.getOrCreatePlugin<RenderView>("renderView");
		renderView.set_transform(Transform(Matrix(Vector(0.92, 0.37, 0.0),
			Vector(0.12, -0.3, 0.94),
			Vector(0.35, -0.87, -0.32)), Vector(59.0, -140, 44)));
		renderView.set_fov(1.65806);
	}
		break;

	case EVrayPluginType::ELightSphere:
	{
		LightSphere lightSphere = renderer.newPlugin<LightSphere>();
		lightSphere.set_transform(Transform(Matrix(Vector(1.0, 0.0, 0.0),
			Vector(0.0, 1.0, 0.0),
			Vector(0.0, 0.0, 1.0)), Vector(0, 0, 200)));
		lightSphere.set_color(Color(1.0, 1.0, 1.0));
		lightSphere.set_intensity(30);
		lightSphere.set_radius(50);
		string temp = lightSphere.getName();
		PluginNameOut = temp.c_str();
	}
	break;
	case EVrayPluginType::ELightRectangle:
	{
		LightRectangle lightRectangle = renderer.newPlugin<LightRectangle>();
		lightRectangle.set_transform(Transform(Matrix(Vector(1.0, 0.0, 0.0),
			Vector(0.0, 1.0, 0.0),
			Vector(0.0, 0.0, 1.0)), Vector(0, 0, 200)));
		lightRectangle.set_color(Color(1.0, 1.0, 1.0));
		lightRectangle.set_intensity(100);
		lightRectangle.set_u_size(128);
		lightRectangle.set_v_size(128);
		string temp = lightRectangle.getName();
		PluginNameOut = temp.c_str();
	}
	break;
	case EVrayPluginType::EAll:
		break;
	default:
		break;
	}
} 

bool AVrayInterface::DeletePluginCpp(FString PluginName)
{
	Plugin plugin = renderer.getPlugin(TCHAR_TO_UTF8(*PluginName));
	if (renderer.deletePlugin(plugin))
	{
		return true;
	}
	return false;
}

void AVrayInterface::SetVrayPluginParameter(bool&ParamSetSuccessfully, EVrayPluginType PluginType, TArray<FVector>transformIn, FString nameIn, FLinearColor colorIn, int32 intIn, TArray<float>floatArrayIn, FString ParameterName)
{
	bool valueFound;
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
	case EVrayPluginType::ELightSphere:
	{
		LightSphere lightSphere = renderer.getPlugin<LightSphere>(TCHAR_TO_UTF8(*nameIn));
		plugin = lightSphere;
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
	case EVrayPluginType::ECamera:
	{
		//ignora nome, pois somente uma instancia eh permitida
		CameraPhysical cameraPhysical = renderer.getInstanceOf<CameraPhysical>();
		cameraPhysical.set_specify_fov(true);
		cameraPhysical.set_fov(1);
		plugin = cameraPhysical;
		renderer.setCamera(cameraPhysical);

	}
		break;
	case EVrayPluginType::ESettingsCamera:
	{
		SettingsCamera settingsCamera = renderer.getInstanceOf<SettingsCamera>();
		plugin = settingsCamera;
	}
	case EVrayPluginType::EAll:
		break;
	default:
		break;
	}

	Type paramType = plugin.getValue(TCHAR_TO_UTF8(*ParameterName), valueFound).getType();

	if (valueFound)
	{
		switch (paramType)
		{
		case VRay::TYPE_INT:
		{
			ParamSetSuccessfully = (plugin.setValue(TCHAR_TO_UTF8(*ParameterName), intIn));
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
			Color color; 
			color.r = colorIn.R;
			color.g = colorIn.R;
			color.b = colorIn.G;
			ParamSetSuccessfully = (plugin.setValue(TCHAR_TO_UTF8(*ParameterName), color));
		}
		break;
		case VRay::TYPE_ACOLOR:
		{
			AColor aColor;
			aColor.color.r = colorIn.R;
			aColor.color.g = colorIn.R;
			aColor.color.b = colorIn.G;
			aColor.alpha = 1.0;
			ParamSetSuccessfully = (plugin.setValue(TCHAR_TO_UTF8(*ParameterName), aColor));
		}
		break;
		case VRay::TYPE_MATRIX:
			break;
		case VRay::TYPE_TRANSFORM:
		{
			VRay::Transform t=plugin.getTransform("transform");
			t.offset.x = transformIn[3].X;
			t.offset.y = transformIn[3].Y;
			t.offset.z = transformIn[3].Z;
			if (plugin.setValue(TCHAR_TO_UTF8(*ParameterName), t))
			{
				ParamSetSuccessfully = true;
			}
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

void AVrayInterface::GetVrayPluginParameter(TArray<FString>&propertyNamesOut, TArray<FString>&PropertyValuesOut, TArray<FString>&ParamTypeOut, EVrayPluginType PluginType, TArray<FVector>&transformOut, FString nameIn, FLinearColor&colorOut, int32&intOut, TArray<float>&floatArrayOut, bool&paramFound, FString ParameterName, FString&ParameterValue)
{
	//se ParameterName vazio, retorna os parametros do plugin ignorando plugin type
	vector<string>propertyNames;
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
	case EVrayPluginType::ELightSphere:
	{
		LightSphere lightSphere = renderer.getPlugin<LightSphere>(TCHAR_TO_UTF8(*nameIn));
		plugin = lightSphere;
	}
	break;
	case  EVrayPluginType::ECamera:
	{
		plugin = renderer.getInstanceOf<CameraPhysical>();
	}
	break;

	case EVrayPluginType::ESettingsCamera:
	{
		plugin = renderer.getInstanceOf<SettingsCamera>();
	}
	case EVrayPluginType::ELightRectangle:
	{
		LightRectangle lightRectangle = renderer.getPlugin<LightRectangle>(TCHAR_TO_UTF8(*nameIn));
		plugin = lightRectangle;
	}
	break;
	case EVrayPluginType::EAll:
	{
		UE_LOG(LogTemp, Warning, TEXT("listing all plugins: "));
		for (const Plugin&plugin : renderer.getPlugins())
		{
			tempString = plugin.getName();
			tempFString = tempString.c_str();
			UE_LOG(LogTemp, Warning, TEXT("(%s)"),*tempFString);
		}
	}
	break;
	}
	if (plugin.isNotEmpty())
	{
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
				floatArrayOut[0] = plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getFloat();
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
				AColor acolor = plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getAColor();
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
					transformOut[i].X = transform.matrix[i].x;
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
			case VRay::TYPE_TEXTURETRANSFORM:;
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
			propertyNamesOut.Empty();
			PropertyValuesOut.Empty();
			ParamTypeOut.Empty();
			PluginMeta pluginMeta = renderer.getPluginMeta(plugin.getType());
			propertyNames = pluginMeta.getPropertyNames();
			for (size_t i = 0; i < propertyNames.size(); i++)
			{
				propertyNamesOut.Push(propertyNames[i].c_str());
				tempString = plugin.getValueAsString(propertyNames[i]);
				PropertyValuesOut.Push(tempString.c_str());
				tempString = plugin.getValue(propertyNames[i]).getStringType();
				ParamTypeOut.Push(tempString.c_str());
			}
		}
	}
}

bool AVrayInterface::CreateGeomStaticMesh(bool box, TArray<FVector>UnrealVertices, TArray<FVector>UnrealNormals, TArray<int32>UnrealFaces, TArray<int32>UnrealFaceNormals, FString NodeName)
{
	GeomStaticMesh mesh = renderer.newPlugin<GeomStaticMesh>();
	vector<Vector>vertices;
	vector<int>facess;
	vector<Vector>normals;
	vector<int>faceNormals;
	Vector tempVector;
	if (box)
	{
		vertices = { (
			Vector(-9.84252, -9.84252, 0),
			Vector(9.84252, -9.84252, 0),
			Vector(-9.84252, 9.84252, 0),
			Vector(9.84252, 9.84252, 0),
			Vector(-9.84252, -9.84252, 19.68504),
			Vector(9.84252, -9.84252, 19.68504),
			Vector(-9.84252, 9.84252, 19.68504),
			Vector(9.84252, 9.84252, 19.68504)
		) };

		facess = { (0, 2, 3, 3, 1, 0, 4, 5, 7, 7, 6, 4, 0, 1, 5, 5, 4, 0, 1, 3, 7, 7, 5, 1, 3, 2, 6, 6, 7, 3, 2, 0, 4, 4, 6, 2) };

		normals = { (
			Vector(0, 0, -1),
			Vector(0, -1, 0),
			Vector(-1, 0, 0),
			Vector(0, 0, -1),
			Vector(0, -1, 0),
			Vector(1, 0, 0),
			Vector(0, 0, -1),
			Vector(0, 1, 0),
			Vector(-1, 0, 0),
			Vector(0, 0, -1),
			Vector(1, 0, -0),
			Vector(0, 1, 0),
			Vector(0, 0, 1),
			Vector(0, -1, 0),
			Vector(-1, 0, 0),
			Vector(0, -0, 1),
			Vector(0, -1, 0),
			Vector(1, -0, 0),
			Vector(-0, 0, 1),
			Vector(0, 1, 0),
			Vector(-1, -0, -0),
			Vector(0, 0, 1),
			Vector(1, 0, 0),
			Vector(0, 1, 0)
		) };

		faceNormals = { (0, 6, 9, 9, 3, 0, 12, 15, 21, 21, 18, 12, 1, 4, 16, 16, 13, 1, 5, 10, 22, 22, 17, 5, 11, 7, 19, 19, 23, 11, 8, 2, 14, 14, 20, 8) };

		mesh.set_vertices(vertices);
		mesh.set_faces(facess);
		mesh.set_normals(normals);
		mesh.set_faceNormals(faceNormals);
		Node node = renderer.getPlugin<Node>(TCHAR_TO_UTF8(*NodeName));
		node.set_geometry(mesh);
		return 0;
	}
	else
	{
		for (size_t i = 0; i < UnrealVertices.Num(); i++)
		{
			tempVector.set(UnrealVertices[i].X, UnrealVertices[i].Y, UnrealVertices[i].Z);
			vertices.push_back(tempVector);
		}
		for (size_t i = 0; i < UnrealFaces.Num(); i++)
	{
		facess.push_back(UnrealFaces[i]);
	}
		for (size_t i = 0; i < UnrealNormals.Num(); i++)
	{
		tempVector.set(UnrealNormals[i].X, UnrealNormals[i].Y, UnrealNormals[i].Z);
		normals.push_back(tempVector);
	}
		for (size_t i = 0; i < UnrealFaceNormals.Num(); i++)
	{
		faceNormals.push_back(UnrealFaceNormals[i]);
	}
	}
	mesh.set_vertices(vertices);
	mesh.set_faces(facess);
	mesh.set_normals(normals);
	mesh.set_faceNormals(faceNormals);
	Node node = renderer.getPlugin<Node>(TCHAR_TO_UTF8(*NodeName));
	node.set_geometry(mesh);
	return 0;
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

int32 AVrayInterface::commit()
{
	return(renderer.commit());
}

void AVrayInterface::LoadScene(FString path)
{
	// "C:\\Users\\master\\Documents\\3ds Max 2020\\scenes\\cenaBase.vrscene");
	renderer.load(TCHAR_TO_UTF8(*path));
}

void AVrayInterface::Render(int option)
{
	switch (option)
	{
		case 0:
		{
			
			VRayRenderer::VFB& vfb = renderer.vfb;
			vfb.show(true /*show*/, false /*setFocus*/);     // The window is visible and auto focused
			vfb.setPositionAndSize(-800, 450, 800, 600);         // Position in screen-space and size in pixels
			//vfb.enableInteractivity(false);                  // Whether camera mouse control is enabled
			vfb.setAlwaysOnTop(true);                       // Toggles always-on-top window behavior
			//size_t numBytes = 0;
			//VFBState *stateBuffer = vfb.getState(numBytes); // Can be used to save serialized window state
			//vfb.setState(stateBuffer, numBytes);


			
			SettingsGI gi = renderer.getInstanceOrCreate<SettingsGI>();
			gi.set_on(true);
			gi.set_primary_engine(2);
			gi.set_secondary_engine(3);

			renderer.setAutoCommit(true);
			renderer.startSync();
			renderer.setKeepInteractiveRunning(true);

		}
		break;
		case 1:
		{
		}
		break;
		case 2:
		{
		}
		break;
	}
}


