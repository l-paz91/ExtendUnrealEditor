// copyright longPointer* 2023

// -----------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include <Framework/Docking/TabManager.h>
#include <Widgets/Docking/SDockTab.h>

// -----------------------------------------------------------------------------

class FLongToolsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void InitCBMenuExtension();
	void AddCBMenuEntry(FMenuBuilder& InMenuBuilder);
	void OnDeleteUnusedAssetButtonClicked();
	void OnDeleteEmptyFoldersMenuEntryClicked();
	void OnAdvancedDeletionButtonClicked();
	void FixUpRedirectors();

	void RegisterAdvancedDeletionTab();
	TSharedRef<SDockTab> OnSpawnAdvancedDeletionTab(const FSpawnTabArgs& SpawnTabArgs);
	TArray<TSharedPtr<FAssetData>> GetAllAssetDataUnderSelectedFolder();

	TSharedRef<FExtender> CustomCBMenuExtender(const TArray<FString>& InSelectedPaths);

	TArray<FString> mFolderPathsSelected;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
