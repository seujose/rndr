#include "VrayInterface.h"
#include "RndrAssimp.h"
#include "vraysdk.hpp"
#include "vrayplugins.hpp"
#include <vector>
#include <string>
#include "Engine/Texture2D.h"
#include "VrayInterface.h"
using namespace VRay;
using namespace VRay::Plugins;
using namespace std;

void fn_render(VRayRenderer&renderer, int32 renderMode, int32 timeToStop, float noise)
{
	renderer.stop();
	VRayRenderer::VFB& vfb = renderer.vfb;
	vfb.enableProgressBar(true);
	vfb.show(true /*show*/, true /*setFocus*/);     // The window is visible and auto focused
	vfb.setPositionAndSize(1287, 448, 640, 640);         // Position in screen-space and size in pixels
	vfb.enableInteractivity(true);
	vfb.setAlwaysOnTop(true);
	renderer.setImageSize(256, 256, true, true);

	SettingsRTEngine settingsRTEngine = renderer.getInstanceOrCreate<SettingsRTEngine>();
	settingsRTEngine.set_undersampling(5);

	SettingsGI settingsGI = renderer.getInstanceOrCreate<SettingsGI>();
	settingsGI.set_on(true);
	settingsGI.set_primary_engine(2);
	settingsGI.set_secondary_engine(3);

	renderer.setInteractiveNoiseThreshold(noise);
	renderer.setInteractiveTimeout(timeToStop);

	switch (renderMode)
	{
	case 0:
	{
		renderer.setRenderMode(VRayRenderer::RENDER_MODE_PRODUCTION);
		UE_LOG(LogTemp, Warning, TEXT("RENDER_MODE_PRODUCTION"));
	}
	break;

	case 1:
	{
		renderer.setRenderMode(VRayRenderer::RENDER_MODE_INTERACTIVE);
		UE_LOG(LogTemp, Warning, TEXT("RENDER_MODE_INTERACTIVE"));
	}
	break;

	case 2:
	{
		renderer.setRenderMode(VRayRenderer::RENDER_MODE_INTERACTIVE_CUDA);
		UE_LOG(LogTemp, Warning, TEXT("RENDER_MODE_INTERACTIVE_CUDA"));
	}
	break;

	case 3:
	{
		renderer.setRenderMode(VRayRenderer::RENDER_MODE_PRODUCTION_CUDA);
		UE_LOG(LogTemp, Warning, TEXT("RENDER_MODE_PRODUCTION_CUDA"));
	}
	break;

	}
	
	renderer.setAutoCommit(true);
	renderer.startSync();
	renderer.setKeepInteractiveRunning(true);
}

VRay::VRayInit init(true);

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

void AVrayInterface::CreatePluginCpp(FString&PluginNameOut, EVrayPluginType PluginType, bool twoSidedMat)
{
	switch (PluginType)
	{
	case EVrayPluginType::ENode:
	{
		Node node = renderer.newPlugin<Node>();
		if (twoSidedMat)
		{
			VRay::Plugins::Mtl2Sided twoSidedMat = renderer.newPlugin<Mtl2Sided>();
			twoSidedMat.set_mult_by_front_diffuse(true);
			twoSidedMat.set_force_1sided(true);
			twoSidedMat.set_translucency_tex_mult(0);
			MtlSingleBRDF matFront = renderer.newPlugin<MtlSingleBRDF>();
			MtlSingleBRDF matBack = renderer.newPlugin<MtlSingleBRDF>();
			BRDFVRayMtl brdfFront = renderer.newPlugin<BRDFVRayMtl>();
			BRDFVRayMtl brdfBack = renderer.newPlugin<BRDFVRayMtl>();
			brdfFront.set_opacity_color(AColor(0, 1, 0, 1));
			brdfBack.set_opacity_color(AColor(1, 0, 0, 1));
			matFront.set_brdf(brdfFront);
			matBack.set_brdf(brdfBack);
			twoSidedMat.set_front(matFront);
			twoSidedMat.set_back(matBack);
			brdfFront.set_opacity(1);
			brdfFront.set_opacity_mode(2);
			brdfFront.set_diffuse(AColor(0, 1, 0, 1));
			brdfBack.set_diffuse(AColor(1, 0, 0, 1));
			node.set_material(twoSidedMat);
		}
		else
		{
			MtlSingleBRDF singleMat = renderer.newPlugin<MtlSingleBRDF>();
			BRDFVRayMtl brdf = renderer.newPlugin<BRDFVRayMtl>();
			singleMat.set_brdf(brdf);
			BitmapBuffer bitmapBuffer = renderer.newPlugin<BitmapBuffer>();
			TexBitmap texBitmap = renderer.newPlugin<TexBitmap>();
			texBitmap.set_bitmap(bitmapBuffer);
			brdf.set_diffuse(texBitmap);
			node.set_material(singleMat);
		}
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
	case EVrayPluginType::EPhysicalCamera:
	{
		//somente uma instancia
		CameraPhysical cameraPhysical = renderer.getOrCreatePlugin<CameraPhysical>(NULL);
		//cameraPhysical.set_exposure(true);
		cameraPhysical.set_specify_fov(true);
		//cameraPhysical.set_fov(45*3.14/180);
		cameraPhysical.set_fov(0.5);
		string temp = cameraPhysical.getName();
		PluginNameOut = temp.c_str();
	}
	break;

	case EVrayPluginType::ERenderView:
	{
		//somente uma instancia
		RenderView renderView = renderer.getInstanceOrCreate<RenderView>();
		//renderView.set_transform(Transform(Matrix(Vector(0, 0, 0.0),Vector(0.12, -0.3, 0.94),Vector(0.35, -0.87, -0.32)), Vector(59.0, -140, 44)));
		
		string temp = renderView.getName();
		PluginNameOut = temp.c_str();
	}
		break;

	case EVrayPluginType::ELightSphere:
	{
		LightSphere lightSphere = renderer.newPlugin<LightSphere>();
		lightSphere.set_transform(Transform(Matrix(Vector(1.0, 0.0, 0.0),
			Vector(0.0, 1.0, 0.0),
			Vector(0.0, 0.0, 1.0)), Vector(0, 0, 200)));
		//lightSphere.set_color(Color(1.0, 1.0, 1.0));
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
			Vector(0.0, 0.0, 1.0)), Vector(0, 0, 100)));
		//lightRectangle.set_color(Color(1.0, 1.0, 1.0));
		lightRectangle.set_intensity(30);
		lightRectangle.set_u_size(64);
		lightRectangle.set_v_size(64);
		string temp = lightRectangle.getName();
		PluginNameOut = temp.c_str();
	}
	break;
	case EVrayPluginType::EGenericPlugin:
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

void AVrayInterface::SetVrayPluginParameter(FMatrix theMatrix, bool&ParamSetSuccessfully, EVrayPluginType PluginType, TArray<FVector>transformIn, FString nameIn, FLinearColor colorIn, int32 intIn, TArray<float>floatArrayIn, FString ParameterName, bool resyncRender, FString stringIn, bool boolin)
{
	resyncRender = false;
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
		/*if (ParameterName=="diffuse")
		{
			AVrayInterface::editMaterial(stringIn, nameIn, colorIn);
		}
		return;*/
	}
		break;
	case EVrayPluginType::ERenderView:
	{
		RenderView renderView = renderer.getInstanceOf<RenderView>();
		plugin = renderView;
	}
		break;

	case EVrayPluginType::EPhysicalCamera:
	{
		CameraPhysical cameraPhysical = renderer.getInstanceOrCreate<CameraPhysical>();
		plugin = cameraPhysical;
		resyncRender = true;
	}
	break;

	case EVrayPluginType::EGenericPlugin:
	{
		plugin = renderer.getPlugin(TCHAR_TO_UTF8(*nameIn));
	}
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
		case VRay::TYPE_BOOL:
		{
			ParamSetSuccessfully = (plugin.setValue(TCHAR_TO_UTF8(*ParameterName), boolin));
		}
			break;
		case VRay::TYPE_COLOR:
		{
			Color color; 
			color.r = colorIn.R;
			color.g = colorIn.G;
			color.b = colorIn.B;
			ParamSetSuccessfully = (plugin.setValue(TCHAR_TO_UTF8(*ParameterName), color));
		}
		break;
		case VRay::TYPE_ACOLOR:
		{
			if (stringIn.IsEmpty())
			{
				AColor aColor;
				aColor.color.r = colorIn.R;
				aColor.color.g = colorIn.G;
				aColor.color.b = colorIn.B;
				aColor.alpha = colorIn.A;
				ParamSetSuccessfully = (plugin.setValue(TCHAR_TO_UTF8(*ParameterName), aColor));
			}
			else
			{
				BitmapBuffer bitmapBuffer = renderer.newPlugin<BitmapBuffer>();
				bitmapBuffer.set_file(TCHAR_TO_UTF8(*stringIn));
				TexBitmap texBitmap = renderer.newPlugin<TexBitmap>();
				texBitmap.set_bitmap(bitmapBuffer);
				ParamSetSuccessfully = (plugin.setValue(TCHAR_TO_UTF8(*ParameterName), texBitmap));
			}
		}
		break;
		case VRay::TYPE_TRANSFORM:
		{

			VRay::Transform t;
			t.matrix[0].set(transformIn[0].X, transformIn[0].Y, transformIn[0].Z);
			t.matrix[1].set(transformIn[1].X, transformIn[1].Y, transformIn[1].Z);
			t.matrix[2].set(transformIn[2].X, transformIn[2].Y, transformIn[2].Z);
			t.offset.set(transformIn[3].X, transformIn[3].Y, transformIn[3].Z);


			if (plugin.setValue(TCHAR_TO_UTF8(*ParameterName), t))
			{
				ParamSetSuccessfully = true;
			}
		}
		break;
		case VRay::TYPE_PLUGIN:
		{
			if (!stringIn.IsEmpty())
			{
				BitmapBuffer bitmapBuffer = renderer.newPlugin<BitmapBuffer>();
				bitmapBuffer.set_file(TCHAR_TO_UTF8(*stringIn));
				TexBitmap texBitmap = renderer.newPlugin<TexBitmap>();
				texBitmap.set_bitmap(bitmapBuffer);
				ParamSetSuccessfully = (plugin.setValue(TCHAR_TO_UTF8(*ParameterName), texBitmap));
			}
			else
			{
				AColor aColor;
				aColor.color.r = colorIn.R;
				aColor.color.g = colorIn.G;
				aColor.color.b = colorIn.B;
				aColor.alpha = colorIn.A;
				ParamSetSuccessfully = (plugin.setValue(TCHAR_TO_UTF8(*ParameterName), aColor));
			}
		}
		break;
		}

		if (resyncRender)
		{
			renderer.commit();
			renderer.startSync();
			RenderView renderView = renderer.getInstanceOf<RenderView>();
			renderView.set_transform(renderView.get_transform());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s not found"), *ParameterName);
	}
}

void AVrayInterface::GetVrayPluginParameter(TArray<FString>&propertyNamesOut, TArray<FString>&PropertyValuesOut,
	TArray<FString>&ParamTypeOut, EVrayPluginType PluginType, TArray<FVector>&tOut, FString nameIn, 
	FLinearColor&colorOut, int32&intOut, TArray<float>&floatArrayOut, bool&paramFound, 
	FString ParameterName, FString&ParameterValueAsString)
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
	case  EVrayPluginType::ERenderView:
	{
		plugin = renderer.getInstanceOf<RenderView>();
	}
	break;

	case EVrayPluginType::EPhysicalCamera:
	{
		plugin = renderer.getInstanceOf< CameraPhysical>();
	}
	break;
	case EVrayPluginType::ELightRectangle:
	{
		LightRectangle lightRectangle = renderer.getPlugin<LightRectangle>(TCHAR_TO_UTF8(*nameIn));
		plugin = lightRectangle;
	}
	break;
	case  EVrayPluginType::ETexBitmap:
	{
		Node node = renderer.getPlugin<Node>(TCHAR_TO_UTF8(*nameIn));
		MtlSingleBRDF mat = renderer.getPlugin<MtlSingleBRDF>(node.get_material().getName());
		BRDFVRayMtl brdf = plugin_cast<BRDFVRayMtl>(mat.get_brdf());
		TexBitmap texBitmap = plugin_cast<TexBitmap>(brdf.get_diffuse().as<Plugin>());
		BitmapBuffer bitmapBuffer = plugin_cast<BitmapBuffer>(texBitmap.get_bitmap());
		plugin = bitmapBuffer;
	}
	break;
	case EVrayPluginType::EGenericPlugin:
	{
		plugin = renderer.getPlugin(TCHAR_TO_UTF8(*nameIn));
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

				tempString = plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getStringType();
				ParamTypeOut.Push(tempString.c_str());
			}
			break;
			case VRay::TYPE_TRANSFORM://#otimizar
			{
				VRay::Transform t = plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getTransform();
				FVector temp;
				tOut.Init(temp, 4);
				tOut[0].Set(t.matrix[0].x, t.matrix[0].y, t.matrix[0].z);
				tOut[1].Set(t.matrix[1].x, t.matrix[1].y, t.matrix[1].z);
				tOut[2].Set(t.matrix[2].x, t.matrix[2].y, t.matrix[2].z);
				tOut[3].X = t.offset.x;
				tOut[3].Y = t.offset.y;
				tOut[3].Z = t.offset.z;
			}
			break;
			case VRay::TYPE_STRING:
			{
				tempString = plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getStringType();
				ParamTypeOut.Push(tempString.c_str());

				ParameterValueAsString = plugin.getValueAsString(TCHAR_TO_UTF8(*ParameterName)).c_str();
			}
				break;
			case VRay::TYPE_PLUGIN:
			{
				tempString = plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).getStringType();
				ParamTypeOut.Push(tempString.c_str());

				Plugin childPlugin = plugin.getValue(TCHAR_TO_UTF8(*ParameterName)).as<Plugin>();
				tempString = childPlugin.getName();
				ParameterValueAsString = tempString.c_str();
			}
				break;
			}
		}
		else
		{
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

void AVrayInterface::CreateGeomStaticMesh(TArray<FVector2D>UVChannel1, TArray<FVector2D>UVChannel2, TArray<FVector>UnrealVertices, TArray<FVector>UnrealNormals, TArray<int32>UnrealFaces, TArray<int32>UnrealFaceNormals, FString NodeName)
{
	GeomStaticMesh mesh = renderer.newPlugin<GeomStaticMesh>();
	vector<Vector>vertices;
	vector<int>facess;
	vector<Vector>normals;
	vector<int>faceNormals;
	Vector tempVector;
	vector<Vector>uvws;

	ValueList first_channel;
	ValueList second_Channel;
	first_channel.push_back(Value(1));          
	second_Channel.push_back(Value(2));
	 
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

	ValueList map_channels;
	for (size_t i = 0; i < UVChannel1.Num(); i++)
	{
		tempVector.set(UVChannel1[i].X, UVChannel1[i].Y, (0,0, 0,0, 0,0));
		uvws.push_back(tempVector);
	}
	first_channel.push_back(Value(VectorList(uvws)));
	first_channel.push_back(Value(IntList(facess)));
	map_channels.push_back(Value(first_channel));

	uvws.clear();
	if (UVChannel2.Num()>0)
	{
		for (size_t i = 0; i < UVChannel2.Num(); i++)
		{
			tempVector.set(UVChannel2[i].X, UVChannel2[i].Y, (0, 0, 0, 0, 0, 0));
			uvws.push_back(tempVector);
		}
		second_Channel.push_back(Value(VectorList(uvws)));
		second_Channel.push_back(Value(IntList(facess)));
		map_channels.push_back(Value(second_Channel));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("(%s) uv channel two invalid"), *NodeName);
	}

	mesh.set_map_channels(map_channels);
	mesh.set_vertices(vertices);
	mesh.set_faces(facess);
	mesh.set_normals(normals);
	mesh.set_faceNormals(faceNormals);
	Node node = renderer.getPlugin<Node>(TCHAR_TO_UTF8(*NodeName));
	node.set_geometry(mesh);
}

void AVrayInterface::editMaterial(FString bitMapPath, FString nodeName, FLinearColor color)
{
	Node node = renderer.getPlugin<Node>(TCHAR_TO_UTF8(*nodeName));
	MtlSingleBRDF mat = renderer.getPlugin<MtlSingleBRDF>(node.get_material().getName());
	BRDFVRayMtl brdf = plugin_cast<BRDFVRayMtl>(mat.get_brdf());
	if (bitMapPath.IsEmpty())
	{
		brdf.set_diffuse(AColor(color.R, color.G, color.B, color.A)); //define a cor
	}
	else
	{
		//#implementar otimizacao para nao criar estes plugins caso ja exista
		/*if (brdf.get_diffuse().getType()==TYPE_PLUGIN)
		{
			TexBitmap texBitmap = plugin_cast<TexBitmap>(brdf.get_diffuse().as<Plugin>());
			BitmapBuffer bitmapBuffer = plugin_cast<BitmapBuffer>(texBitmap.get_bitmap());
		}*/
		BitmapBuffer bitmapBuffer = renderer.newPlugin<BitmapBuffer>();
		bitmapBuffer.set_file(TCHAR_TO_UTF8(*bitMapPath));
		TexBitmap texBitmap = renderer.newPlugin<TexBitmap>();
		texBitmap.set_bitmap(bitmapBuffer);
		brdf.set_diffuse(texBitmap);
	}
	/*
		if (!(brdf.get_diffuse().as<Plugin>()).isEmpty())//if plugin is not empty
		{
			TexBitmap texBitmap = plugin_cast<TexBitmap>(brdf.get_diffuse().as<Plugin>());
			BitmapBuffer bitmapBuffer = plugin_cast<BitmapBuffer>(texBitmap.get_bitmap());
			if (texBitmap.isNotEmpty() && bitmapBuffer.isNotEmpty())//se ja existem os plugins de textura
			{
				bitmapBuffer.set_file(TCHAR_TO_UTF8(*bitMapPath));
				texBitmap.set_bitmap(bitmapBuffer);
			}
			else
			{
				BitmapBuffer bitmapBuffer = renderer.newPlugin<BitmapBuffer>();
				bitmapBuffer.set_file(TCHAR_TO_UTF8(*bitMapPath));
				TexBitmap texBitmap = renderer.newPlugin<TexBitmap>();
				texBitmap.set_bitmap(bitmapBuffer);
			}
		}
	}*/
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

void AVrayInterface::getRawImage(UTexture2D*&outTexture, bool preserveAlpha/*=true*/)
{
	VRayImage *image = renderer.getImage();
	if (image)
	{
		size_t bufferSize=0;
		int32 imgSizeW, imgSizeH;
		image->getSize(imgSizeW, imgSizeH);
		UTexture2D *tempTexture = UTexture2D::CreateTransient(imgSizeW, imgSizeH);//falta pixel format
		//tempTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		//tempTexture->SRGB = 0;
		tempTexture->AddToRoot();
		tempTexture->UpdateResource();
		FTexture2DMipMap &mip = tempTexture->PlatformData->Mips[0];
		void *data = mip.BulkData.Lock(LOCK_READ_WRITE);
		image->toBitmapData(bufferSize);
		FMemory::Memcpy(data, image->toBitmapData(bufferSize, preserveAlpha, false, false), bufferSize);
		mip.BulkData.Unlock();
		tempTexture->UpdateResource();
		outTexture = tempTexture;
	}
}

void AVrayInterface::getGeoInfo(FString PluginName, TArray<FVector>&VerticesOut, TArray<FVector>&NormalsOut, 
	TArray<int32>&FacesOut, TArray<int32>&facesNormalsOut, TArray<FVector2D>&UVZeroOut, TArray<FVector2D>&UVOneOut, TArray<int32>&mapChannelfacesOut)
{
	GeomStaticMesh geo = plugin_cast<GeomStaticMesh>(renderer.getPlugin<Node>(TCHAR_TO_UTF8(*PluginName)).get_geometry());
	
	for (const Vector&i:geo.get_vertices())
	{
		VerticesOut.Add({ i.x, i.y, i.z });
	}
	for (const Vector&i : geo.get_normals())
	{
		NormalsOut.Add({ i.x, i.y, i.z });
	}
	for (const int&i : geo.get_faces())
	{
		FacesOut.Add(i);
	}
	for (const int&i : geo.get_faceNormals())
	{
		facesNormalsOut.Add(i);
	}
	if (geo.get_map_channels().size() > 0)
	{
		switch (geo.get_map_channels().size())
		{
		case 2:
		{
			VectorList mapChannelVerticesZero = geo.get_map_channels()[0][1].as<VectorList>();
			for (const Vector&i : mapChannelVerticesZero)
			{
				UVZeroOut.Add({ i.x, i.y });
			}
			VectorList mapChannelVerticesOne = geo.get_map_channels()[1][1].as<VectorList>();
			for (const Vector&i : mapChannelVerticesOne)
			{
				UVOneOut.Add({ i.x, i.y });
			}
		}
		break;
		case 1:
		{
			VectorList mapChannelVerticesZero = geo.get_map_channels()[0][1].as<VectorList>();
			for (const Vector&i : mapChannelVerticesZero)
			{
				UVZeroOut.Add({ i.x, i.y });
			}
		}
		break;
		}
	}
}

void AVrayInterface::updateView(TArray<FVector>T)
{
	VRay::Transform t;
	t.matrix[0].set(T[0].X, T[0].Y, T[0].Z);
	t.matrix[1].set(T[1].X, T[1].Y, T[1].Z);
	t.matrix[2].set(T[2].X, T[2].Y, T[2].Z);
	t.offset.set(T[3].X, T[3].Y, T[3].Z);

	RenderView renderView = renderer.getInstanceOf<RenderView>();
	renderView.set_transform(t);
}

void AVrayInterface::bakeAnode(FString nodeName, int32 mode, int32 channelToUse)
{
	RenderElements reManager = renderer.getRenderElements();
	if ((reManager.getAll(RenderElement::RAWLIGHT).size()) ==0)
	{
		reManager.add(RenderElement::RAWLIGHT, NULL, NULL);
	}
	BakeView theBaker = renderer.newPlugin<BakeView>();
	Node theTarget = renderer.getPlugin<Node>(TCHAR_TO_UTF8(*nodeName));
	theBaker.set_bake_node(theTarget);
	UVWGenChannel theUVs = renderer.newPlugin<UVWGenChannel>();
	theUVs.set_uvw_channel(channelToUse);
	theBaker.set_bake_uvwgen(theUVs);
	ValueList theTargetNodeList;
	theTargetNodeList.push_back((Value(theTarget)));
	theBaker.set_target_nodes(theTargetNodeList);
	theBaker.set_dilation(2);
	theBaker.set_u_min(0);
	theBaker.set_v_min(0);
	theBaker.set_u_max(1);
	theBaker.set_v_max(1);
	RenderElement rawLightRE = renderer.getRenderElements().get(RenderElement::RAWLIGHT);
	string basePath = "C:/rndr/baked_";
	string baseName = string(TCHAR_TO_UTF8(*nodeName));
	string finalPath = basePath + baseName+ "_"+rawLightRE.getName()+".png";
	fn_render(renderer, mode, 5000, 0.1);
	renderer.waitForRenderEnd();
	if (rawLightRE)
	{
		Plugin rawLithPlugin = rawLightRE.getPlugin();
		VRayImage *image = rawLightRE.getImage();
		if (image)
		{
			image->saveToPng(finalPath);
			delete image;
		}
	}
}

void AVrayInterface::LoadScene(FString path)
{
	renderer.load(TCHAR_TO_UTF8(*path));
} 

void AVrayInterface::Render(int renderMode)
{
	fn_render(renderer, renderMode, 0, 0.1);
}


