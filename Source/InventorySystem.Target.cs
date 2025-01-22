// Copyright by liclem97.

using UnrealBuildTool;
using System.Collections.Generic;

public class InventorySystemTarget : TargetRules
{
	public InventorySystemTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "InventorySystem" } );
	}
}
