// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class UnrealGames : ModuleRules
{
	public UnrealGames(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RenderCore", "RHI", "OnlineSubsystem", "OnlineSubsystemUtils"});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Online features
		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem" });
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
        if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		{
			if (UEBuildConfiguration.bCompileSteamOSS == true)
			{
				DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
			}
		}

        if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            PublicAdditionalLibraries.Add("ThirdParty/Steamworks/Steamv132/sdk/redistributable_bin/steam_api.lib");
        }
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add("ThirdParty/Steamworks/Steamv132/sdk/redistributable_bin/win64/steam_api64.lib");
        }
        
    }
}
