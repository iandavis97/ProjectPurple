// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectPurpleEditorTarget : TargetRules
{
	public ProjectPurpleEditorTarget(TargetInfo Target): base(Target)
	{
		Type = TargetType.Editor;
        
        ExtraModuleNames.Add("ProjectPurple");
	}

/*
	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.AddRange( new string[] { "ProjectPurple" } );
	}*/
}
