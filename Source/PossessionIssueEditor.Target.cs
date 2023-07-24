using UnrealBuildTool;

public class PossessionIssueEditorTarget : TargetRules
{
	public PossessionIssueEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("PossessionIssue");
	}
}
