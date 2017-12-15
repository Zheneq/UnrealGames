// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UnrealGamesEditorTarget : TargetRules
{
	public UnrealGamesEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        bUsesSteam = true;
        ExtraModuleNames.Add("UnrealGames");
	}
}
