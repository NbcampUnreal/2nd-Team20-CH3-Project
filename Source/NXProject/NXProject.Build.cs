// NXProject.Build.cs

using UnrealBuildTool;

public class NXProject : ModuleRules
{
	public NXProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			// Initial Dependencies
			 "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem", "GameplayTasks", "AnimationCore", "UMG", "Niagara", "NiagaraCore"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { "Niagara" });
		
		PublicIncludePaths.AddRange(new string[] { "NXProject" });
	}
}
