// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class testOnly : ModuleRules
{
	public testOnly(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
        PublicAdditionalLibraries.Add("C:\\rndr\\testOnly\\Binaries\\Win64\\assimp-vc140-mt.lib");
    }
}
