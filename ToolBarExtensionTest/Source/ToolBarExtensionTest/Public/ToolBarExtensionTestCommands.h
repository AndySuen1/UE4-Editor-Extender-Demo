// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ToolBarExtensionTestStyle.h"

class FToolBarExtensionTestCommands : public TCommands<FToolBarExtensionTestCommands>
{
public:

	FToolBarExtensionTestCommands()
		: TCommands<FToolBarExtensionTestCommands>(TEXT("ToolBarExtensionTest"), NSLOCTEXT("Contexts", "ToolBarExtensionTest", "ToolBarExtensionTest Plugin"), NAME_None, FToolBarExtensionTestStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
