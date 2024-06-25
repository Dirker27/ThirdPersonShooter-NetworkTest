// (C) ToasterCat Studios 2024

using UnrealBuildTool;
using System.Collections.Generic;

public class TPS_MultiplayerEditorTarget : TargetRules
{
	public TPS_MultiplayerEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "TPS_Multiplayer" } );
	}
}
