// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class CustomControllers : ModuleRules
	{
		public CustomControllers(TargetInfo Target)
		{
            PrivateIncludePaths.AddRange(new string[] { "CustomControllers/Private" });
            PublicIncludePaths.AddRange(new string[] { "CustomControllers", "CustomControllers/Public" });

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
				    "Core", 
				    "CoreUObject", 
				    "Engine"
				}
				);
		}
	}
}