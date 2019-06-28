// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Casino_IncTarget : TargetRules
{
	public Casino_IncTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("Casino_Inc");

		bUseUnityBuild = false;
		bUsePCHFiles = false;
	}
}
