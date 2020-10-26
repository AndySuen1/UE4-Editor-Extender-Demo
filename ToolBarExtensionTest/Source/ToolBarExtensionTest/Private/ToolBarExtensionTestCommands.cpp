// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToolBarExtensionTestCommands.h"

#define LOCTEXT_NAMESPACE "FToolBarExtensionTestModule"

void FToolBarExtensionTestCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "ToolBarExtensionTest", "Execute ToolBarExtensionTest action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
