using UnrealBuildTool;

public class PossessionIssueTarget : TargetRules
{
	public PossessionIssueTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("PossessionIssue");
	}
}
