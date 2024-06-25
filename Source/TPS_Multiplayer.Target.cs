// (C) ToasterCat Studios 2024

using UnrealBuildTool;
using System.Collections.Generic;

public class TPS_MultiplayerTarget : TargetRules
{
	public TPS_MultiplayerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "TPS_Multiplayer" } );
	}
}
