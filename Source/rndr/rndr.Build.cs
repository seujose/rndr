// Fill out your copyright notice in the Description page of Project Settings.


//https://answers.unrealengine.com/questions/70019/how-to-include-the-header-file-from-a-plugin.html
//https://answers.unrealengine.com/questions/70019/how-to-include-the-header-file-from-a-plugin.html

using UnrealBuildTool;
using System.IO;

public class rndr : ModuleRules
{
	public rndr(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EditableMesh" });
        PrivateDependencyModuleNames.AddRange(new string[] {});
        //PublicIncludePaths.AddRange(new string[] { "E:\\UnrealEngine\\Engine\\Plugins\\Runtime\\EditableMesh\\Source\\EditableMesh\\Public",  "C:\\rndr\\Source\\assimp", "C:\\Program Files\\Chaos Group\\V-Ray\\AppSDK\\cpp\\include" });
        PublicIncludePaths.AddRange(new string[] { "C:\\rndr\\Source\\assimp", "C:\\Program Files\\Chaos Group\\V-Ray\\AppSDK\\cpp\\include" });
        PublicAdditionalLibraries.AddRange(new string[] { "C:\\rndr\\Binaries\\Win64\\assimp-vc140-mt.lib", "C:\\Program Files\\Chaos Group\\V-Ray\\AppSDK\\cpp\\lib\\VRaySDKLibrary.lib" });
    }
}
