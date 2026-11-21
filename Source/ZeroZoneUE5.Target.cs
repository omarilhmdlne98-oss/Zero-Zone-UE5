// Copyright Zero-Zone. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ZeroZoneUE5Target : TargetRules
{
	public ZeroZoneUE5Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("ZeroZoneUE5");
	}
}
