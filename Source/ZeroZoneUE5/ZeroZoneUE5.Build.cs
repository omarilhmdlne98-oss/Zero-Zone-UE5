using UnrealBuildTool;

public class ZeroZoneUE5 : ModuleRules
{
    public ZeroZoneUE5(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NetCore", "WebSockets", "Json", "JsonUtilities", "AIModule", "GameplayTasks", "Niagara" });
        if (Target.bBuildEditor) PrivateDependencyModuleNames.AddRange(new[] { "UnrealEd", "Landscape" });
    }
}
