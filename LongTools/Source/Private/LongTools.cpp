// Copyright Epic Games, Inc. All Rights Reserved.

#include "LongTools.h"

#include <Editor/ContentBrowser/Public/ContentBrowserModule.h>

// -----------------------------------------------------------------------------

#define LOCTEXT_NAMESPACE "FLongToolsModule"

// -----------------------------------------------------------------------------

void FLongToolsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// The exact timing is specified in the .uplugin file per-module
	InitCBMenuExtension();
}

// -----------------------------------------------------------------------------

void FLongToolsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

// -----------------------------------------------------------------------------

void FLongToolsModule::InitCBMenuExtension()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	TArray<FContentBrowserMenuExtender_SelectedPaths>& CBContextMenuExtenders = ContentBrowserModule.GetAllPathViewContextMenuExtenders();

	FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;
	CustomCBMenuDelegate.BindRaw(this, &FLongToolsModule::CustomCBMenuExtender);
	CBContextMenuExtenders.Add(CustomCBMenuDelegate);


}

// -----------------------------------------------------------------------------

void FLongToolsModule::AddCBMenuEntry(FMenuBuilder& InMenuBuilder)
{
	InMenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Delete Unused Assets")),
		FText::FromString(TEXT("Safely delete all unused assets under folder")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FLongToolsModule::OnDeleteUnusedAssetButtonClicked)
	);
}

// -----------------------------------------------------------------------------

void FLongToolsModule::OnDeleteUnusedAssetButtonClicked()
{

}

// -----------------------------------------------------------------------------

TSharedRef<FExtender> FLongToolsModule::CustomCBMenuExtender(const TArray<FString>& InSelectedPaths)
{
	TSharedRef<FExtender> MenuExtender = MakeShareable(new FExtender());

	if (InSelectedPaths.Num())
	{
		MenuExtender->AddMenuExtension(
			FName("Delete"),
			EExtensionHook::After,
			TSharedPtr<FUICommandList>(),
			FMenuExtensionDelegate::CreateRaw(this, &FLongToolsModule::AddCBMenuEntry)
		);
	}

	return MenuExtender;
}

// -----------------------------------------------------------------------------

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLongToolsModule, LongTools)

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
