using UnrealBuildTool;

public class OutcomeModule : ModuleRules
{
    public OutcomeModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "OutcomeModule"
        });

        PublicIncludePaths.AddRange(new string[] { "OutcomeModule/Public" });
        PrivateIncludePaths.AddRange(new string[] { "OutcomeModule/Private" });
    }
}