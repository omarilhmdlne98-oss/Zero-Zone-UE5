// Copyright Zero-Zone. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ZeroZoneUE5EditorTarget : TargetRules
{
	public ZeroZoneUE5EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("ZeroZoneUE5");
	}
}
