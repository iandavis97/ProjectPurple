// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectPurpleTarget : TargetRules
{
	public ProjectPurpleTarget(TargetInfo Target): base(Target)
	{
		Type = TargetType.Game;
        ExtraModuleNames.Add("ProjectPurple");
	}

	//
	// TargetRules interface.
	//

	/*public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.AddRange( new string[] { "ProjectPurple" } );
	}*/
}
