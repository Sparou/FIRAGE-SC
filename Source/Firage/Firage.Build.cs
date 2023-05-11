// Shoot Them Up Game, All Rights Reserved.

using UnrealBuildTool;

public class Firage : ModuleRules
{
    public Firage(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "Niagara",
            "PhysicsCore",
            "GameplayTasks",
            "NavigationSystem",
            "EnhancedInput"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PrivateDependencyModuleNames.AddRange(new string[] { "EnhancedInput" });

        PublicIncludePaths.AddRange(new string[] 
        {
            "Firage/Public/Player",
            "Firage/Public/Components",
            "Firage/Public/Dev",
            "Firage/Public/Weapon",
            "Firage/Public/UI",
            "Firage/Public/Animations",
            "Firage/Public/Pickups",
            "Firage/Public/Weapon/Components",
            "Firage/Public/AI",
            "Firage/Public/AI/Tasks",
            "Firage/Public/AI/Services",
            "Firage/Public/AI/EQS",
            "Firage/Public/AI/Decorators",
            "Firage/Public/Menu",
            "Firage/Public/Menu/UI",
            "Firage/Public/Sound"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
