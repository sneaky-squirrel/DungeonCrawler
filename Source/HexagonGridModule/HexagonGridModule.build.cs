using UnrealBuildTool;

public class HexagonGridModule : ModuleRules
{
    public HexagonGridModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HexagonGridModule"
        });

        PublicIncludePaths.AddRange(new string[] { "HexagonGridModule/Public" });
        PrivateIncludePaths.AddRange(new string[] { "HexagonGridModule/Private" });
    }
}