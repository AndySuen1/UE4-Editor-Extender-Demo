// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToolBarExtensionTest.h"
#include "ToolBarExtensionTestStyle.h"
#include "ToolBarExtensionTestCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "LevelEditor.h"

static const FName ToolBarExtensionTestTabName("ToolBarExtensionTest");

#define LOCTEXT_NAMESPACE "FToolBarExtensionTestModule"

void FToolBarExtensionTestModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FToolBarExtensionTestStyle::Initialize();
	FToolBarExtensionTestStyle::ReloadTextures();

	FToolBarExtensionTestCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FToolBarExtensionTestCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FToolBarExtensionTestModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FToolBarExtensionTestModule::RegisterMenus));

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender());
		ToolbarExtender->AddToolBarExtension("Compile", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FToolBarExtensionTestModule::AddToolbarExtension));
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

}
void FToolBarExtensionTestModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddComboButton(
		FUIAction(),
		FOnGetContent::CreateRaw(this, &FToolBarExtensionTestModule::OpenPulldownMenu),
		LOCTEXT("Combo", "Combo"),
		LOCTEXT("ComboTip", "ComboTip"),
		FSlateIcon(), true);
}

TSharedRef<SWidget> FToolBarExtensionTestModule::OpenPulldownMenu()
{
	TSharedPtr<FMenuBuilder> MenuBuilder = MakeShareable(new FMenuBuilder(true, PluginCommands));
	MenuBuilder->AddMenuEntry(LOCTEXT("Text", "Text"), LOCTEXT("ComboTip", "ComboTip"), FSlateIcon(), FUIAction());
	return MenuBuilder->MakeWidget();
}

void FToolBarExtensionTestModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FToolBarExtensionTestStyle::Shutdown();

	FToolBarExtensionTestCommands::Unregister();
}

void FToolBarExtensionTestModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FToolBarExtensionTestModule::PluginButtonClicked()")),
							FText::FromString(TEXT("ToolBarExtensionTest.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FToolBarExtensionTestModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FToolBarExtensionTestCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FToolBarExtensionTestCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FToolBarExtensionTestModule, ToolBarExtensionTest)