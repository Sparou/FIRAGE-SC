// Shoot Them Up Game, All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FirageTarget : TargetRules
{
	public FirageTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Firage" } );
	}
}
