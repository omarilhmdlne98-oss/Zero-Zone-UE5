// Copyright Zero-Zone. All Rights Reserved.

using UnrealBuildTool;

public class ZeroZoneUE5 : ModuleRules
{
	public ZeroZoneUE5(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"NetCore",
			"WebSockets",
			"Json",
			"JsonUtilities",
			"AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});
	}
}
